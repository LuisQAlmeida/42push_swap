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
	int	valid_ra;
	int	valid_rb;

	valid_ra = (stacks->a.size > 1);
	valid_rb = (stacks->b.size > 1);
	if (!valid_ra && !valid_rb)
		return ;
	if (!valid_ra && valid_rb)
		rot_b(stacks, print);
	else if (valid_ra && !valid_rb)
		rot_a(stacks, print);
	else
	{
		rot_a(stacks, 0);
		rot_b(stacks, 0);
		if (print)
			write(1, "rr\n", 3);
	}
}
