/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:16:12 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 16:58:47 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_exec_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
}

static char	*resolve_from_env(char *cmd, t_shell *shell)
{
	char	*path;

	path = find_cmd_path(cmd, shell->envp);
	if (!path)
	{
		print_exec_error(cmd);
		ft_putendl_fd("command not found", 2);
		exit(127);
	}
	return (path);
}

static char	*resolve_direct_path(char *arg, struct stat *st)
{
	if (stat(arg, st) != 0)
	{
		print_exec_error(arg);
		ft_putendl_fd("No such file or directory", 2);
		exit(127);
	}
	if (S_ISDIR(st->st_mode))
	{
		print_exec_error(arg);
		ft_putendl_fd("Is a directory", 2);
		exit(126);
	}
	if (access(arg, X_OK) == -1)
	{
		print_exec_error(arg);
		ft_putendl_fd("Permission denied", 2);
		exit(126);
	}
	return (ft_strdup(arg));
}

char	*resolve_path(t_cmd *cmd, t_shell *shell)
{
	struct stat	st;

	if (ft_strchr(cmd->argv[0], '/') != NULL)
		return (resolve_direct_path(cmd->argv[0], &st));
	return (resolve_from_env(cmd->argv[0], shell));
}
