#******************************************************#
#			VARIABLES                      #
#******************************************************#
CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = push_swap

SRC = src/push_swap.c src/stack_utils.c src/node_utils.c src/error_utils.c src/parse_utils.c src/validation_utils.c src/swap_ops.c src/push_ops.c src/rot_ops.c src/rrot_ops.c
SRC_BONUS = src/push_swap.c src/stack_utils.c src/node_utils.c src/error_utils.c src/parse_utils.c src/validation_utils.c src/swap_ops.c src/push_ops.c src/rot_ops.c src/rrot_ops.c

SRC_USED = $(SRC)

ifeq ($(BONUS), 1)
SRC_USED = $(SRC_BONUS)
endif

OBJ_USED = $(SRC_USED:.c=.o)

OBJ_ALL = $(SRC:.c=.o) $(SRC_BONUS:.c=.o)

INCLUDE = -Iinclude -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

#******************************************************#
#                       RULES                          #
#******************************************************#
all: $(NAME)

bonus:
	$(MAKE) BONUS=1 all

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJ_USED) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ_USED) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ_ALL)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
