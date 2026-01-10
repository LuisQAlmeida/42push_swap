#include "push_swap.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static void	spaces_sign(const char *str, int *i, long *sign)
{
	while (ft_isspace(str[*i]))
		(*i)++;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
}

int	ft_atoi_ps(const char *str, t_stacks *stacks)
{
	int		i;
	long	result;
	long	sign;

	if (!str)
		error_exit(stacks, ERR_PARSE);
	i = 0;
	result = 0;
	sign = 1;
	spaces_sign(str, &i, &sign);
	if (!str[i] || !ft_isdigit(str[i]))
		error_exit(stacks, ERR_PARSE);
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		if (sign * result < INT_MIN || sign * result > INT_MAX)
			error_exit(stacks, ERR_OVERFLOW);
		i++;
	}
	while (ft_isspace(str[i]))
		i++;
	if (str[i] != '\0')
		error_exit(stacks, ERR_PARSE);
	return ((int)(sign * result));
}

void	ft_split_free(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
