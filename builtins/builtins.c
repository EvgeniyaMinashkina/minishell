/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:13:26 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 16:23:18 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtin_export(char **argv, t_shell *shell)
{
	int		i;
	char	*eq;
	int		status;

	if (!argv[1])
		return (builtin_env(shell->envp));
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
			status = (export_error(argv[i]), 1);
		else
		{
			eq = ft_strchr(argv[i], '=');
			if (eq)
			{
				*eq = '\0';
				env_set(&shell->envp, argv[i], eq + 1);
				*eq = '=';
			}
		}
		i++;
	}
	return (status);
}

/*
static int	builtin_export(char **argv, t_shell *shell) //TODO
{
	char	*eq;

	if (!argv[1])
		return (builtin_env(shell->envp));
	eq = ft_strchr(argv[1], '=');
	if (!eq)
		return (0);
	*eq = '\0';
	env_set(&shell->envp, argv[1], eq + 1);
	*eq = '=';
	return (0);
}*/

static int	builtin_unset(char **argv, t_shell *shell)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		env_unset(&shell->envp, argv[i]);
		i++;
	}
	return (0);
}
/*
static int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;

	if (!argv[1])
		path = env_get(shell->envp, "HOME");
	else
		path = argv[1];
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}*/

/*
** Exit builtin
** Handles:
** - exit            -> exit with current status
** - exit N          -> exit with status N
** - exit N arg      -> error: too many arguments
** Shell is not terminated when too many arguments are provided
*/
static int	builtin_exit(char **argv, t_shell *shell)
{
	int	status;

	printf("exit\n");
	if (argv[1] && !is_valid_exit_number(argv[1]))
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		shell_exit(shell, 2);
	}
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("exit: too many arguments", 2);
		shell->exit_status = 1;
		return (1);
	}
	if (argv[1])
		status = ft_atoi(argv[1]);
	else
		status = shell->exit_status;
	shell_exit(shell, status % 256);
	return (0);
}

/*
** Execute builtin
*/
int	exec_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!ft_strncmp(cmd->argv[0], "pwd", 4))
		return (builtin_pwd());
	if (!ft_strncmp(cmd->argv[0], "echo", 5))
		return (builtin_echo(cmd->argv));
	if (!ft_strncmp(cmd->argv[0], "env", 4))
		return (builtin_env(shell->envp));
	if (!ft_strncmp(cmd->argv[0], "export", 7))
		return (builtin_export(cmd->argv, shell));
	if (!ft_strncmp(cmd->argv[0], "unset", 6))
		return (builtin_unset(cmd->argv, shell));
	if (!ft_strncmp(cmd->argv[0], "cd", 3))
		return (builtin_cd(cmd->argv, shell));
	if (!ft_strncmp(cmd->argv[0], "exit", 5))
		return (builtin_exit(cmd->argv, shell));
	return (0);
}
