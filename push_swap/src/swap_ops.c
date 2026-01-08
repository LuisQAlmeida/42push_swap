#include "push_swap.h"

static void	stack_swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;

	if (!stack || stack->size < 2)
		return ;
	first = stack->top;
	second = stack->next;
	first->next = second->next;
	if (second->next)
		second->next->prev = first;
	second->prev = NULL;
	second->next = first;
	first->prev = second;
	stack->top = second;
	if (stack->bot == second)
		stack->bot = first;
}

void	swap_a(t_stacks *stacks, int print)
{
	stack_swap(&stacks->a);
	if (print)
		write(1, "sa\n", 3);
}

void	swap_b(t_stacks *stacks, int print)
{
	stack_swap(&stacks->b);
	if (print)
		write(1, "sb\n", 3);
}

void	swap_s(t_stacks *stacks, int print)
{
	stack_swap(&stacks->a);
	stack_swap(&stacks->b);
	if (print)
		write(1, "ss\n", 3);
}
