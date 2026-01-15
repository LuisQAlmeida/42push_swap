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

static int	get_min_idx_pos(t_stack *stack)
{
	int		cur_pos;
	int		min_idx;
	int		min_idx_pos;
	t_node	*node;

	node = stack->top;
	cur_pos = 0;
	min_idx = node->index;
	min_idx_pos = 0;
	while (node)
	{
		if (node->index < min_idx)
		{
			min_idx_pos = cur_pos;
			min_idx = node->index;
		}
		node = node->next;
		cur_pos++;
	}
	return (min_idx_pos);
}

static void	place_min_idx_top(t_stacks *stacks)
{
	int	i;
	int	size;
	int	min_idx_pos;

	i = 0;
	size = stacks->a.size;
	min_idx_pos = get_min_idx_pos(&stacks->a);
	if (min_idx_pos <= size / 2)
	{
		while (i++ < min_idx_pos)
			rot_a(stacks, 1);
	}
	else
		while (i++ < size - min_idx_pos)
			rrot_a(stacks, 1);
}

void	sort_five(t_stacks *stacks)
{
	if (stacks->a.size <= 3)
	{
		sort_three(stacks);
		return ;
	}
	while (stacks->a.size > 3)
	{
		place_min_idx_top(stacks);
		push_b(stacks, 1);
	}
	sort_three(stacks);
	while (stacks->b.size > 0)
		push_a(stacks, 1);
}
