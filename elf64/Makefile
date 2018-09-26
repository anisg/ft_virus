NAME = woody_woodpacker

SRC = infector.c virus.c

COMPILER = gcc
FLAGS = 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
BIN_DIR				= bin

OBJ	=	$(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

VIRUS_WRAPPER_START = $(OBJ_DIR)/virus_start.o

VIRUS_C = $(OBJ_DIR)/virus.o

VIRUS_START = virus_start.s
VIRUS_C_TEMPLATE = virus.c

LD_RULES = virus_linking.lds

all: $(NAME)

$(NAME): .tmp/obj/infector.o .tmp/obj/lib/lib.o $(VIRUS_X)
	$(COMPILER) $(FLAGS) -o $(NAME) .tmp/obj/infector.o .tmp/obj/lib/lib.o $(VIRUS_X)

$(VIRUS_X): $(VIRUS)
	cp $(VIRUS) virus_shellcode
	xxd -i virus_shellcode > $(VIRUS_X)
	rm virus_shellcode

$(VIRUS): .tmp/obj/virus.o .tmp/obj/lib/lib.o .tmp/obj/virus_start.o .tmp/obj/lib/remote.o $(VIRUS_C)
	ld -o $@ -T $(LD_RULES) .tmp/obj/virus.o .tmp/obj/virus_start.o .tmp/obj/lib/remote.o .tmp/obj/lib/lib.o

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
