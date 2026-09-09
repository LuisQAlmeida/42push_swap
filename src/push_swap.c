#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stacks	stacks;

	if (argc == 1)
		return (0);
	stack_init(&stacks.a);
	stack_init(&stacks.b);
	parse_args(&stacks, argc, argv);
	if (stacks.a.size == 0 || check_sort(&stacks.a))
	{
		stack_clear(&stacks.a);
		stack_clear(&stacks.b);
		return (0);
	}
	index_compress(&stacks);
	if (stacks.a.size <= 3)
		sort_three(&stacks);
	else if (stacks.a.size <= 5)
		sort_five(&stacks);
	else
		sort_radix(&stacks);
	stack_clear(&stacks.a);
	stack_clear(&stacks.b);
	return (0);
}
