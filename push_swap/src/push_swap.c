#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stacks	stacks;

	if (argc == 1)
		return (0);
	stack_init(&stacks.a);
	stack_init(&stacks.b);
	parse_args(&stacks, argc, argv);
	if (check_sort(&stacks.a))
	{
		stack_clear(&stacks->a);
		stack_clear(&stacks->b);
		return (0);
	}
	stack_clear(&stacks->a);
	stack_clear(&stacks->b);
	return (0);
}
