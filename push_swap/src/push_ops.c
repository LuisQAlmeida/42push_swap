#include "push_swap.h"

void	push_a(t_stacks *stacks, int print)
{
	t_node	*node;

	node = stack_pop_top(&stacks->b);
	if (!node)
		return ;
	stack_push_top(&stacks->a, node);
	if (print)
		write(1, "pa\n", 3);
}

void	push_b(t_stacks *stacks, int print)
{
	t_node	*node;

	node = stack_pop_top(&stacks->a);
	if (!node)
		return ;
	stack_push_top(&stacks->b, node);
	if (print)
		write(1, "pb\n", 3);
}
