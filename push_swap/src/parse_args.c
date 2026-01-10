#include "push_swap.h"

static void	parse_single_arg(t_stacks *stacks, char *str)
{
	int		i;
	int		value;
	char	**nodes;
	t_node	*node;

	nodes = ft_split(str, ' ');
	if (!split)
		error_exit(stacks, ERR_MALLOC);
	i = 0;
	while (nodes[i])
	{
		if (nodes[i][0])
		{
			value = ft_atoi_ps(nodes[i], stacks);
			node = node_init(value);
			if (!node)
			{
				ft_split_free(nodes);
				error_exit(stacks, ERR_MALLOC);
			}
			stack_push_bot(&stacks->a, node);
		}
		i++;
	}
	ft_split_free(nodes);
}

void	parse_args(t_stacks *stacks, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		parse_single_arg(stacks, argv[i]);
		i++;
	}
}
