#include "push_swap.h"

int	check_dups(t_stack *stack)
{
	t_node	*cur;
	t_node	*nxt;

	if (!stack)
		return (0);
	cur = stack->top;
	while (cur)
	{
		nxt = cur->nxt;
		while (nxt)
		{
			if (nxt->value == cur->value)
				return (1);
			nxt = nxt->next;
		}
		cur = cur->nxt;
	}
	return (0);
}

int	check_sort(t_stack *stack)
{
	t_node	*cur;

	if (!stack || stack->size < 2)
		return (1);
	cur = stack->top;
	while (cur->next)
	{
		if (cur->value > cur->next->value)
			return (0);
		cur = cur->next;
	}
	return (1);
}
