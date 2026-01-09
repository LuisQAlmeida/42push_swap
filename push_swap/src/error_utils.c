#include "push_swap.h"

void	error_exit(t_stacks *stacks, t_error error)
{
	if (stacks)
	{
		stack_clear(&stacks->a);
		stack_clear(&stacks->b);
	}
	write(2, "Error\n", 6);
	exit((int)error);
}
