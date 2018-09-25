NAME = woody_woodpacker

SRC = infector.c

COMPILER = gcc
FLAGS = 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
BIN_DIR				= bin

OBJ	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

VIRUS = $(TMP_DIR)/virus.template
VIRUS_X = $(TMP_DIR)/virus_shellcode.c

VIRUS_WRAPPER_START = $(TMP_DIR)/virus_wrapper_start.o

VIRUS_C = $(TMP_DIR)/virus.o

VIRUS_START = $(BIN_DIR)/virus_start.s
VIRUS_C_TEMPLATE = virus.c

LD_RULES = $(BIN_DIR)/virus_linking.lds

all: $(NAME)

$(NAME): $(OBJ) $(VIRUS_X)
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ) $(VIRUS_X)

$(VIRUS_X): $(VIRUS)
	cp $(VIRUS) virus_shellcode
	xxd -i virus_shellcode > $(VIRUS_X)
	rm virus_shellcode

$(VIRUS): $(VIRUS_WRAPPER_START) $(VIRUS_C)
	ld -o $@ -T $(LD_RULES) $(VIRUS_WRAPPER_START) $(VIRUS_C)

$(VIRUS_WRAPPER_START): $(VIRUS_START) | $(TMP_DIR)
	nasm -f elf64 $< -o $@


$(VIRUS_C): $(VIRUS_C_TEMPLATE)
	gcc -MD -fno-stack-protector -fPIC -fPIE -c $< -o $@

$(TMP_DIR):
	mkdir -p $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(COMPILER) -MD $(FLAGS) -c $< -o $@

clean:
	rm -rf .tmp/virus.d .tmp/obj/infector.d
	rm -rf $(OBJ)
	rm -df $(OBJ_DIR)
	rm -rf $(TMP_DIR)

fclean: clean
	rm -df $(VIRUS)
	rm -rf $(NAME)

re: fclean
	$(MAKE)

-include .tmp/virus.d .tmp/obj/infector.d

.PHONY: all re clean fclean
