#include "libft.h"

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

void	stack_push_top(t_stack *stack, t_node *node)
{
	if (!stack || !node)
		return ;
	node->prev = NULL;
	node->next = stack->top;
	if (stack->top)
		stack->top->prev = node;
	stack->top = node;
	if (stack->bot == NULL)
		stack->bot = node;
	stack->size++;
}

void	stack_push_bot(t_stack *stack, t_node *node)
{
	if (!stack || !node)
		return ;
	node->next = NULL;
	node->prev = stack->bot;
	if (stack->bot)
		stack->bot->next = node;
	stack->bot = node;
	if (stack->top == NULL)
		stack->top = node;
	stack->size++;
}

t_node	*node_init(int value)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->value = value;
	node->index = -1;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}
