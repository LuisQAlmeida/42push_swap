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

static void	*sort_int_array(int *array, int size)
{
	int	i;
	int	j;
	int	key;

	i = 1;
	while (i < size)
	{
		key = array[i];
		j = i - 1;
		while (j >= 0 && array[j] > key)
		{
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = key;
		i++;
	}
}
