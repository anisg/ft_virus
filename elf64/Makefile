NAME = infector
COLOR = "\033[92m"

SRC = main.c

COMPILER = gcc
FLAGS = -Wextra -Wall -Werror

SRC_DIR 			= src
OBJ_DIR				= obj

OBJ	=	$(addprefix $(OBJ_DIR)/, $(notdir $(SRC:.c=.o)))

all: $(NAME)

mprint:
	@echo $(COLOR)"make" $(NAME) "\033[0m"
clprint:
	@echo $(COLOR)"clean" $(NAME) "\033[0m"
clfprint:
	@echo $(COLOR)"fclean" $(NAME) "\033[0m"

$(NAME): $(OBJ)
	@echo '----------'
	$(COMPILER) $(FLAGS) -o $(NAME) $(OBJ) prog

$(OBJ): | $(OBJ_DIR)

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

fclean: clean
	@echo '----------'
	@$(MAKE) clfprint
	rm -rf $(NAME)

re: fclean all
