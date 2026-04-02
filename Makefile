ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME	:= libft_malloc_$(HOSTTYPE).so
SYMLINK	:= libft_malloc.so
CC		:= cc
FLG		:= -Wextra -Werror -Wextra -g3 -fPIC #-fsanitize=address,leak

SRC		:= main.c

OBJ_DIR	:= obj/
OBJ		:= $(SRC:%.c=$(OBJ_DIR)%.o)

all: $(NAME)
	@ln -sf $(NAME) $(SYMLINK)

$(NAME): $(OBJ)
	@$(CC) -shared -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLG) -c $< -o $@

clean: 
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(SYMLINK)

re: fclean all


.PHONY: all clean fclean re