/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:16:12 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/12 14:17:20 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_exit(t_shell *shell, int status)
{
	shell_exit(shell, status);
}

/*
** Fork + execute one command
** Handles:
** - redirections
** - builtins inside pipeline
** - execve for external commands
*/
int	execute_command(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		init_signals_child();
		if (!cmd || !cmd->argv || !cmd->argv[0])
			child_exit(shell, 0);
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) < 0)
				child_exit(shell, 1);
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) < 0)
				child_exit(shell, 1);
			close(out_fd);
		}
		if (apply_redirections(cmd->redirs, shell))
			child_exit(shell, shell->exit_status);
		if (is_builtin(cmd->argv[0]))
			child_exit(shell, exec_builtin(cmd, shell));
		path = find_cmd_path(cmd->argv[0], shell->envp);
		if (!path)
		{
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			child_exit(shell, 127);
		}
		execve(path, cmd->argv, shell->envp);
		perror("execve");
		free(path);
		child_exit(shell, 126);
	}
	return (pid);
}