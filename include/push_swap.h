/**
 * @file push_swap.h
 * @brief Shared data model and cross-module interface for push_swap.
 *
 * The maintained implementation parses unique signed integers into two
 * doubly linked stacks and emits constrained stack operations that sort
 * stack a in ascending order.
 *
 * Small inputs use dedicated sorting routines. Larger inputs are compressed
 * to integer indices and processed with a binary radix strategy.
 */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>

/* ************************************************************************** */
/*                                Structures                                  */
/* ************************************************************************** */

/**
 * @brief Node stored inside a push_swap stack.
 */
typedef struct s_node
{
	/** @brief Original parsed integer value. */
	int				value;
	/** @brief Compressed sorting index. */
	int				index;
	/** @brief Next node toward the stack bottom. */
	struct s_node	*next;
	/** @brief Previous node toward the stack top. */
	struct s_node	*prev;
}	t_node;

/**
 * @brief Doubly linked stack with direct top and bottom access.
 */
typedef struct s_stack
{
	/** @brief Number of nodes currently stored. */
	int		size;
	/** @brief Top node, or NULL when the stack is empty. */
	t_node	*top;
	/** @brief Bottom node, or NULL when the stack is empty. */
	t_node	*bot;
}	t_stack;

/**
 * @brief Complete push_swap working state.
 */
typedef struct s_stacks
{
	/** @brief Primary stack containing the input and final result. */
	t_stack	a;
	/** @brief Auxiliary stack used by sorting operations. */
	t_stack	b;
}	t_stacks;

/**
 * @brief Error codes propagated by parsing and allocation operations.
 */
typedef enum e_error
{
	/** @brief Successful operation. */
	ERR_NONE = 0,
	/** @brief Invalid input syntax. */
	ERR_PARSE,
	/** @brief Parsed number lies outside the signed int range. */
	ERR_OVERFLOW,
	/** @brief Duplicate integer detected. */
	ERR_DUPLICATE,
	/** @brief Dynamic allocation failed. */
	ERR_MALLOC
}	t_error;

/* ************************************************************************** */
/*                               Stack lifecycle                              */
/* ************************************************************************** */

/**
 * @brief Initialize an empty stack.
 *
 * @param stack Stack to initialize.
 */
void	stack_init(t_stack *stack);

/**
 * @brief Release every node owned by a stack and reset it to empty.
 *
 * @param stack Stack to clear. NULL is accepted.
 */
void	stack_clear(t_stack *stack);

/* ************************************************************************** */
/*                                Node helpers                                */
/* ************************************************************************** */

/**
 * @brief Allocate and initialize one stack node.
 *
 * @param value Original integer value stored by the node.
 * @return Newly allocated node, or NULL on allocation failure.
 */
t_node	*node_init(int value);

/**
 * @brief Remove and return the current top node.
 *
 * The returned node is detached from the list and becomes caller-owned.
 *
 * @param stack Stack from which to pop.
 * @return Detached top node, or NULL when unavailable.
 */
t_node	*stack_pop_top(t_stack *stack);

/**
 * @brief Remove and return the current bottom node.
 *
 * The returned node is detached from the list and becomes caller-owned.
 *
 * @param stack Stack from which to pop.
 * @return Detached bottom node, or NULL when unavailable.
 */
t_node	*stack_pop_bot(t_stack *stack);

/**
 * @brief Insert a detached node at the top of a stack.
 *
 * @param stack Destination stack.
 * @param node Node transferred into the stack.
 */
void	stack_push_top(t_stack *stack, t_node *node);

/**
 * @brief Insert a detached node at the bottom of a stack.
 *
 * @param stack Destination stack.
 * @param node Node transferred into the stack.
 */
void	stack_push_bot(t_stack *stack, t_node *node);

/* ************************************************************************** */
/*                                   Errors                                   */
/* ************************************************************************** */

/**
 * @brief Clear owned stack state, report an error, and terminate.
 *
 * Writes `Error\n` to standard error before exiting with the supplied
 * project error code.
 *
 * @param stacks Working state whose stacks must be released.
 * @param error Error code used as the process exit status.
 */
void	error_exit(t_stacks *stacks, t_error error);

/* ************************************************************************** */
/*                                  Parsing                                   */
/* ************************************************************************** */

/**
 * @brief Parse one numeric token into a signed integer.
 *
 * The function reports parsing and overflow failures without terminating the
 * process, allowing the caller to release memory it owns before handling the
 * error.
 *
 * @param str Token to parse.
 * @param value Output integer written only when conversion succeeds.
 * @return ERR_NONE on success, ERR_PARSE for invalid syntax, or ERR_OVERFLOW
 * when the value lies outside the signed int range.
 */
t_error	ft_atoi_ps(const char *str, int *value);

/**
 * @brief Release a NULL-terminated string array returned by ft_split().
 *
 * @param array Owned string array to release. NULL is accepted.
 */
void	ft_split_free(char **array);

/**
 * @brief Test a stack for duplicate integer values.
 *
 * @param stack Stack to inspect.
 * @return Non-zero when a duplicate exists, otherwise zero.
 */
int		check_dups(t_stack *stack);

/**
 * @brief Test whether a stack is already sorted in ascending value order.
 *
 * @param stack Stack to inspect.
 * @return Non-zero when sorted or containing fewer than two elements.
 */
int		check_sort(t_stack *stack);

/**
 * @brief Parse command-line arguments into stack a.
 *
 * Individual arguments may contain one integer or multiple
 * space-delimited integers. Invalid, duplicate, or overflowing input
 * terminates through error_exit().
 *
 * @param stacks Initialized working state receiving parsed nodes.
 * @param argc Command-line argument count.
 * @param argv Command-line argument vector.
 */
void	parse_args(t_stacks *stacks, int argc, char **argv);

/* ************************************************************************** */
/*                              Stack operations                              */
/* ************************************************************************** */

/**
 * @brief Swap the first two elements of stack a when possible.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `sa\n`.
 */
void	swap_a(t_stacks *stacks, int print);

/**
 * @brief Swap the first two elements of stack b when possible.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `sb\n`.
 */
void	swap_b(t_stacks *stacks, int print);

/**
 * @brief Apply the maintained combined swap operation.
 *
 * When both stacks contain at least two nodes, both are swapped and `ss` is
 * emitted when requested. If only one stack can be swapped, the valid
 * single-stack operation is used instead.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit the operation performed.
 */
void	swap_s(t_stacks *stacks, int print);

/**
 * @brief Push the top node of stack b onto stack a.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `pa\n`.
 */
void	push_a(t_stacks *stacks, int print);

/**
 * @brief Push the top node of stack a onto stack b.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `pb\n`.
 */
void	push_b(t_stacks *stacks, int print);

/**
 * @brief Rotate stack a by moving its top node to the bottom.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `ra\n`.
 */
void	rot_a(t_stacks *stacks, int print);

/**
 * @brief Rotate stack b by moving its top node to the bottom.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `rb\n`.
 */
void	rot_b(t_stacks *stacks, int print);

/**
 * @brief Apply the maintained combined rotate operation.
 *
 * If both stacks can rotate, both are rotated and `rr` is emitted when
 * requested. If only one can rotate, the valid single-stack operation is
 * used instead.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit the operation performed.
 */
void	rot_r(t_stacks *stacks, int print);

/**
 * @brief Reverse rotate stack a by moving its bottom node to the top.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `rra\n`.
 */
void	rrot_a(t_stacks *stacks, int print);

/**
 * @brief Reverse rotate stack b by moving its bottom node to the top.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit `rrb\n`.
 */
void	rrot_b(t_stacks *stacks, int print);

/**
 * @brief Apply the maintained combined reverse-rotate operation.
 *
 * If both stacks can reverse rotate, both are updated and `rrr` is emitted
 * when requested. If only one can operate, the valid single-stack operation
 * is used instead.
 *
 * @param stacks Working stack state.
 * @param print Non-zero to emit the operation performed.
 */
void	rrot_r(t_stacks *stacks, int print);

/* ************************************************************************** */
/*                             Index compression                              */
/* ************************************************************************** */

/**
 * @brief Assign each node its zero-based position in sorted value order.
 *
 * A temporary array is built and sorted before every node receives a unique
 * compressed index in the range `0..n-1`.
 *
 * @param stacks Working state containing stack a.
 */
void	index_compress(t_stacks *stacks);

/* ************************************************************************** */
/*                              Sorting routines                              */
/* ************************************************************************** */

/**
 * @brief Sort stack a when it contains at most three values.
 *
 * @param stacks Working stack state.
 */
void	sort_three(t_stacks *stacks);

/**
 * @brief Sort stack a when it contains at most five values.
 *
 * Values are moved temporarily to stack b until the remaining three can be
 * handled by sort_three(), then restored in sorted order.
 *
 * @param stacks Working stack state.
 */
void	sort_five(t_stacks *stacks);

/**
 * @brief Sort a larger stack using binary radix passes over node indices.
 *
 * Each pass partitions stack a according to one index bit using `pb` and
 * `ra`, then restores stack b using `pa`.
 *
 * @param stacks Working stack state.
 */
void	sort_radix(t_stacks *stacks);

#endif
