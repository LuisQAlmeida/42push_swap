#include "push_swap.h"

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

void	push_a(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->b.size == 0)
		return ;
	node = stack_pop_top(&stacks->b);
	stack_push_top(&stacks->a, node);
	if (print)
		write(1, "pa\n", 3);
}

void	push_b(t_stacks *stacks, int print)
{
	t_node	*node;

	if (stacks->a.size == 0)
		return ;
	node = stack_pop_top(&stacks->a);
	stack_push_top(&stacks->b, node);
	if (print)
		write(1, "pb\n", 3);
}
