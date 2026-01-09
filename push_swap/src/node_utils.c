#include "push_swap.h"

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

t_node	*stack_pop_top(t_stack *stack)
{
	t_node	*node;

	if (!stack || stack->size == 0)
		return (NULL);
	node = stack->top;
	stack->top = node->next;
	if (stack->top)
		stack->top->prev = NULL;
	else
		stack->bot = NULL;
	node->next = NULL;
	node->prev = NULL;
	stack->size--;
	return (node);
}

t_node	*stack_pop_bot(t_stack *stack)
{
	t_node	*node;

	if (!stack || stack->size == 0)
		return (NULL);
	node = stack->bot;
	stack->bot = node->prev;
	if (stack->bot)
		stack->bot->next = NULL;
	else
		stack->top = NULL;
	node->next = NULL;
	node->prev = NULL;
	stack->size--;
	return (node);
}
