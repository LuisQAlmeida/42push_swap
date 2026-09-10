#include "push_swap.h"

static void	parse_node(t_stacks *stacks, char **nodes, int i)
{
	int		value;
	t_node	*node;
	t_error	error;

	error = ft_atoi_ps(nodes[i], &value);
	if (error != ERR_NONE)
	{
		ft_split_free(nodes);
		error_exit(stacks, error);
	}
	node = node_init(value);
	if (!node)
	{
		ft_split_free(nodes);
		error_exit(stacks, ERR_MALLOC);
	}
	stack_push_bot(&stacks->a, node);
}

static void	parse_single_arg(t_stacks *stacks, char *str)
{
	int		i;
	char	**nodes;

	nodes = ft_split(str, ' ');
	if (!nodes)
		error_exit(stacks, ERR_MALLOC);
	if (!nodes[0])
	{
		ft_split_free(nodes);
		error_exit(stacks, ERR_PARSE);
	}
	i = 0;
	while (nodes[i])
	{
		parse_node(stacks, nodes, i);
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
	if (check_dups(&stacks->a))
		error_exit(stacks, ERR_DUPLICATE);
}
