/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:13:26 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/22 20:57:33 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return (1);
	printf("%s\n", cwd);
	return (0);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[1] && !ft_strncmp(argv[1], "-n", 3))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

int	is_valid_exit_number(char *str)
{
	int			sign;
	int			i;
	long long	res;

	i = 0;
	sign = 1;
	res = 0;

	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		if (res > LLONG_MAX / 10
			|| (res == LLONG_MAX / 10
			&& str[i] - '0' > LLONG_MAX % 10 + (sign < 0)))
			return (0);
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

// int	is_valid_exit_number(char *str)
// {
// 	int	i;

// 	i = 0;
// 	if (!str || !str[i])
// 		return (0);
// 	if (str[i] == '+' || str[i] == '-')
// 		i++;
// 	if (!str[i])
// 		return (0);
// 	while (str[i])
// 	{
// 		if (!ft_isdigit(str[i]))
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }
