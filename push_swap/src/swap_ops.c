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
	if (stacks->a.size < 2)
		return ;
	stack_swap(&stacks->a);
	if (print)
		write(1, "sa\n", 3);
}

void	swap_b(t_stacks *stacks, int print)
{
	if (stacks->b.size < 2)
		return ;
	stack_swap(&stacks->b);
	if (print)
		write(1, "sb\n", 3);
}

void	swap_s(t_stacks *stacks, int print)
{
	int	valid_sa;
	int	valid_sb;

	valid_sa = (stacks->a.size > 1);
	valid_sb = (stacks->b.size > 1);
	if (!valid_sa && !valid_sb)
		return ;
	if (!valid_sa && valid_sb)
		swap_b(stacks, print);
	else if (valid_sa && !valid_sb)
		swap_a(stacks, print);
	else
	{
		swap_a(stacks, 0);
		swap_b(stacks, 0);
		if (print)
			write(1, "ss\n", 3);
	}
}
