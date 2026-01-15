#include "push_swap.h"

static int	get_max_bit_count(t_stack *stack)
{
	int	max_idx;
	int	bit_count;

	if (stack->size < 2)
		return (0);
	bit_count = 0;
	max_idx = stack->size - 1;
	while ((max_idx >> bit_count) != 0)
		bit_count++;
	return (bit_count);
}

void	sort_radix(t_stacks *stacks)
{
	int	i;
	int	idx;
	int	size;
	int	cur_bit;
	int	max_bit_count;

	cur_bit = 0;
	max_bit_count = get_max_bit_count(&stacks->a);
	while (cur_bit < max_bit_count)
	{
		i = 0;
		size = stacks->a.size;
		while (i++ < size)
		{
			idx = stacks->a.top->index;
			if (((idx >> cur_bit) & 1) == 0)
				push_b(stacks, 1);
			else
				rot_a(stacks, 1);
		}
		while (stacks->b.size > 0)
			push_a(stacks, 1);
		cur_bit++;
	}
}
