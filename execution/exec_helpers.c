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

void	exit_exec_error(t_cmd *cmd, t_shell *shell)
{
	print_exec_error(cmd->argv[0]);
	if (errno == EACCES)
		ft_putendl_fd("Permission denied", 2);
	else if (errno == EISDIR)
		ft_putendl_fd("Is a directory", 2);
	else if (ft_strchr(cmd->argv[0], '/'))
		ft_putendl_fd("No such file or directory", 2);
	else
		ft_putendl_fd("command not found", 2);
	if (errno == EACCES || errno == EISDIR)
		shell_exit(shell, 126);
	shell_exit(shell, 127);
}

static char	*resolve_from_env(char *cmd, t_shell *shell)
{
	return (find_cmd_path(cmd, shell->envp));
}

static char	*resolve_direct_path(char *arg, struct stat *st)
{
	if (stat(arg, st) != 0)
		return (NULL);
	if (S_ISDIR(st->st_mode))
	{
		errno = EISDIR;
		return (NULL);
	}
	if (access(arg, X_OK) == -1)
	{
		errno = EACCES;
		return (NULL);
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
