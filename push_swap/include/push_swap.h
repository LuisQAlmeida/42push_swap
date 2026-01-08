#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

/******************** DATA STRUCTURES: ********************/
typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_stack
{
	int		size;
	t_node	*top;
	t_node	*bot;
}	t_stack;

typedef struct s_stacks
{
	t_stack	a;
	t_stack	b;
}	t_stacks;

/******************** STACK UTILS *************************/
void	stack_init(t_stack *stack);
void	stack_clear(t_stack *stack);
void	stack_push_top(t_stack *stack, t_node *node);
void	stack_push_bot(t_stack *stack, t_node *node);
t_node	*stack_pop_top(t_stack *stack);
t_node	*stack_pop_bot(t_stack *stack);
t_node	*node_init(int value);

/******************** STACK OPERATIONS: *******************/
void	swap_a(t_stacks *stacks, int print);
void	swap_b(t_stacks *stacks, int print);
void	swap_s(t_stacks *stacks, int print);
void	push_a(t_stacks *stacks, int print);
void	push_b(t_stacks *stacks, int print);
void	rot_a(t_stacks *stacks, int print);
void	rot_b(t_stacks *stacks, int print);
void	rot_s(t_stacks *stacks, int print);
void	rrot_a(t_stacks *stacks, int print);
void	rrot_b(t_stacks *stacks, int print);
void	rrot_s(t_stacks *stacks, int print);

#endif
