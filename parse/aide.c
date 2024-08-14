#include "parser.h"
int	ft_strchr_2(const char *str, int c)
{
	char			*ptr;
	int				i;
	unsigned char	ch;

	i = 0;
	ptr = (char *)str;
	ch = c;
	if (!str)
		return (0);
	while (ptr[i])
	{
		if (ptr[i] == ch)
			return (i);
		i++;
	}
	if (ptr[i] == ch)
		return (i);
	return (0);
}


void	to_exit(void)
{
	ft_putstr_fd("exit\n", 2);
	exit(1);
}

int	is_all_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] != ' ' || str[i] != '\n' || str[i] != '\t'))
			return (0);
		i++;
	}
	return (1);
}