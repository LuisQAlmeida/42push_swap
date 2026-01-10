#include "push_swap.h"

int	has_dups(t_stack *stack)
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
