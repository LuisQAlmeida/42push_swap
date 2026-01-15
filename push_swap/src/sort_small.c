#include "push_swap.h"

static void	sort_three_idx(t_stacks *stacks, int idx_0, int idx_1, int idx_2)
{
	if (idx_1 < idx_0 && idx_0 < idx_2)
		swap_a(stacks, 1);
	else if (idx_1 < idx_2 && idx_2 < idx_0)
		rot_a(stacks, 1);
	else if (idx_2 < idx_0 && idx_0 < idx_1)
		rrot_a(stacks, 1);
	else if (idx_0 < idx_2 && idx_2 < idx_1)
	{
		swap_a(stacks, 1);
		rot_a(stacks, 1);
	}
	else if (idx_2 < idx_1 && idx_1 < idx_0)
	{
		swap_a(stacks, 1);
		rrot_a(stacks, 1);
	}
	else
		return ;
}

void	sort_three(t_stacks *stacks)
{
	int		idx_0;
	int		idx_1;
	int		idx_2;
	t_node	*node;

	if (stacks->a.size < 2)
		return ;
	if (stacks->a.size == 2)
	{
		if (stacks->a.top->index > stacks->a.bot->index)
			swap_a(stacks, 1);
		return ;
	}
	node = stacks->a.top;
	idx_0 = node->index;
	idx_1 = node->next->index;
	idx_2 = node->next->next->index;
	sort_three_idx(stacks, idx_0, idx_1, idx_2);
}
