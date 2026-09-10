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

static t_error	parse_number(const char *str, int i, long sign, int *value)
{
	int		digit;
	long	result;
	long	limit;

	result = 0;
	limit = INT_MAX;
	if (sign < 0)
		limit = -(long)INT_MIN;
	while (ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (result > (limit - digit) / 10)
			return (ERR_OVERFLOW);
		result = result * 10 + digit;
		i++;
	}
	while (ft_isspace(str[i]))
		i++;
	if (str[i] != '\0')
		return (ERR_PARSE);
	*value = (int)(sign * result);
	return (ERR_NONE);
}

t_error	ft_atoi_ps(const char *str, int *value)
{
	int		i;
	long	sign;

	if (!str || !value)
		return (ERR_PARSE);
	i = 0;
	sign = 1;
	spaces_sign(str, &i, &sign);
	if (!str[i] || !ft_isdigit(str[i]))
		return (ERR_PARSE);
	return (parse_number(str, i, sign, value));
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
