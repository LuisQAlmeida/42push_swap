#include "push_swap.h"

void	rrot_a(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->a.size < 2)
		return ;
	node = stack_pop_bot(&stacks->a);
	stack_push_top(&stacks->a, node);
	if (print)
		write(1, "rra\n", 4);
}

void	rrot_b(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->b.size < 2)
		return ;
	node = stack_pop_bot(&stacks->b);
	stack_push_top(&stacks->b, node);
	if (print)
		write(1, "rrb\n", 4);
}

void	rrot_r(t_stacks *stacks, int print)
{
	int	valid_rra;
	int	valid_rrb;

	valid_rra = (stacks->a.size > 1);
	valid_rrb = (stacks->b.size > 1);
	if (!valid_rra && !valid_rrb)
		return ;
	if (!valid_rra && valid_rrb)
		rrot_b(stacks, print);
	else if (valid_rra && !valid_rrb)
		rrot_a(stacks, print);
	else
	{
		rrot_a(stacks, 0);
		rrot_b(stacks, 0);
		if (print)
			write(1, "rrr\n", 4);
	}
}
