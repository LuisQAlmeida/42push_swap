#include "push_swap.h"

static int	*stack_to_array(t_stack *stack, int size)
{
	int		*array;
	int		i;
	t_node	*node;

	if (!stack || size <= 0)
		return (NULL);
	array = (int *)malloc(sizeof(int) * size);
	if (!array)
		return (NULL);
	i = 0;
	node = stack->top;
	while (node && i < size)
	{
		array[i++] = node->value;
		node = node->next;
	}
	return (array);
}

static void	sort_int_array(int *array, int size)
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

static int	index_find(int *array, int size, int value)
{
	int	index;

	index = 0;
	while (index < size)
	{
		if (array[index] == value)
			return (index);
		index++;
	}
	return (-1);
}

void	index_compress(t_stacks *stacks)
{
	int		*array;
	int		size;
	int		index;
	t_node	*node;

	size = stacks->a.size;
	if (size <= 0)
		return ;
	array = stack_to_array(&stacks->a, size);
	if (!array)
		error_exit(stacks, ERR_MALLOC);
	sort_int_array(array, size);
	node = stacks->a.top;
	while (node)
	{
		index = index_find(array, size, node->value);
		if (index >= 0)
			node->index = index;
		node = node->next;
	}
	free(array);
}
