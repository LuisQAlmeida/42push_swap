#include "push_swap.h"

void	rot_a(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->a.size < 2)
		return ;
	node = stack_pop_top(&stacks->a);
	stack_push_bot(&stacks->a, node);
	if (print)
		write(1, "ra\n", 3);
}

void	rot_b(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->b.size < 2)
		return ;
	node = stack_pop_top(&stacks->b);
	stack_push_bot(&stacks->b, node);
	if (print)
		write(1, "rb\n", 3);
}

void	rot_r(t_stacks *stacks, int print)
{
	rot_a(stacks, 0);
	rot_b(stacks, 0);
	if (print)
		write(1, "rr\n", 3);
}
