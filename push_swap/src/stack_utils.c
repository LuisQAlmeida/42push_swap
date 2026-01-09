#include "push_swap.h"

void	stack_init(t_stack *stack)
{
	if (!stack)
		return ;
	stack->size = 0;
	stack->top = NULL;
	stack->bot = NULL;
}

void	stack_clear(t_stack *stack)
{
	t_node	*curr;
	t_node	*next;

	if (!stack)
		return ;
	curr = stack->top;
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	stack_init(stack);
}
