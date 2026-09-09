# **************************************************************************** #
# ****************************** VARIABLES: ********************************** #
# **************************************************************************** #
CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = push_swap

SRC = src/push_swap.c \
	src/stack_utils.c src/node_utils.c src/error_utils.c \
	src/parse_utils.c src/validation_utils.c src/index_utils.c src/parse_args.c \
	src/swap_ops.c src/push_ops.c src/rot_ops.c src/rrot_ops.c \
	src/sort_small.c src/sort_radix.c

OBJ = $(SRC:.c=.o)

INCLUDE = -Iinclude -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# **************************************************************************** #
# ****************************** RULES: ************************************** #
# **************************************************************************** #
all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
