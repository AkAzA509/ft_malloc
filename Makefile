ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME		:= libft_malloc_$(HOSTTYPE).so
SYMLINK		:= libft_malloc.so
CC			:= cc
FLG			:= -Wextra -Werror -Wextra -g3 #-fsanitize=address,leak
fPIC_FLG	:= -fPIC
SRC_PATH	:= src/
INC_PATH	:= include/
TEST_N		:= test/test.c
TEST_T		:= test/test_thread.c
TEST_N_EXE	:= test_normal
TEST_T_EXE	:= test_thread

SRC			:= $(SRC_PATH)malloc.c \
			   $(SRC_PATH)free.c \
			   $(SRC_PATH)realloc.c \
			   $(SRC_PATH)logger.c \
			   $(SRC_PATH)ft_printf/convert_format.c \
			   $(SRC_PATH)ft_printf/ft_printf.c \
			   $(SRC_PATH)ft_printf/utils.c \

OBJ_DIR		:= obj/
OBJ			:= $(SRC:%.c=$(OBJ_DIR)%.o)

all: $(NAME)
	@ln -sf $(NAME) $(SYMLINK)

$(NAME): $(OBJ)
	@$(CC) -shared -o $(NAME) $(OBJ)

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLG) $(fPIC_FLG) -c $< -o $@

test: all
	@$(CC) $(FLG) $(TEST_N) -I$(INC_PATH) -L. -lft_malloc -Wl,-rpath,'$$ORIGIN' -o $(TEST_N_EXE)
	@$(CC) $(FLG) $(TEST_T) -I$(INC_PATH) -L. -lft_malloc -Wl,-rpath,'$$ORIGIN' -o $(TEST_T_EXE)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(TEST_N_EXE) $(TEST_T_EXE)
	@rm -f $(NAME) $(SYMLINK)

re: fclean all

.PHONY: all clean fclean re test