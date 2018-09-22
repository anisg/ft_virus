NAME = infector
COLOR = "\033[92m"

SRC = infector.c

COMPILER = gcc
FLAGS = 

SRC_DIR				= .
TMP_DIR				= .tmp
OBJ_DIR				= $(TMP_DIR)/obj
BIN_DIR				= bin

OBJ	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

VIRUS = $(BIN_DIR)/virus.template

VIRUS_WRAPPER_START = $(TMP_DIR)/virus_wrapper_start.o
VIRUS_WRAPPER_END = $(TMP_DIR)/virus_wrapper_end.o

VIRUS_C = $(TMP_DIR)/virus.o

VIRUS_START = $(BIN_DIR)/virus_start.s
VIRUS_END = $(BIN_DIR)/virus_end.s
VIRUS_C_TEMPLATE = virus.c

LD_RULES = $(BIN_DIR)/virus_linking.lds

all: $(NAME)

mprint:
	@echo $(COLOR)"make" $(NAME) "\033[0m"
clprint:
	@echo $(COLOR)"clean" $(NAME) "\033[0m"
clfprint:
	@echo $(COLOR)"fclean" $(NAME) "\033[0m"

$(NAME): $(OBJ) $(VIRUS)
	@echo '----------'
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ)

$(VIRUS): $(TMP_DIR) $(VIRUS_WRAPPER_START) $(VIRUS_WRAPPER_END) $(VIRUS_C)
	ld -o $@ -T $(LD_RULES) $(VIRUS_WRAPPER_START) $(VIRUS_C) $(VIRUS_WRAPPER_END)

$(VIRUS_WRAPPER_START):
	nasm -f elf64 $(VIRUS_START) -o $@

$(VIRUS_WRAPPER_END):
	nasm -f elf64 $(VIRUS_END) -o $@


$(VIRUS_C):
	gcc -fPIC -c $(VIRUS_C_TEMPLATE) -o $@

$(OBJ): | $(OBJ_DIR)

$(TMP_DIR):
	mkdir -p $@

$(OBJ_DIR):
	@$(MAKE) mprint
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

clean:
	@echo '----------'
	@$(MAKE) clprint
	rm -rf $(OBJ)
	rm -df $(OBJ_DIR)
	rm -rf $(TMP_DIR)

fclean: clean
	@echo '----------'
	rm -df $(VIRUS)
	@$(MAKE) clfprint
	rm -rf $(NAME)

re: fclean all
