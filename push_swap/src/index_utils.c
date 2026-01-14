#include "push_swap.h"

static int	*stack_to_array(t_stack *stack, int size)
{
	int		*array;
	int		i;
	t_node	*node;

	array = (int *)malloc(sizeof(int) * size);
	if (!array)
		return (NULL);
	i = 0;
	node = stack->top;
	while (node)
	{
		array[i] = node->value;
		node = node->next;
		i++;
	}
	return (array);
}
