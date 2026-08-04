/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:15:42 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/04 15:08:38 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_parent_builtin(t_cmd *cmd, t_shell *shell)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	if (save_stdio(&stdin_copy, &stdout_copy))
		return (1);
	if (apply_redirections(cmd->redirs, shell))
	{
		restore_stdio(stdin_copy, stdout_copy);
		shell->exit_status = 1;
		return (1);
	}
	status = exec_builtin(cmd, shell);
	restore_stdio(stdin_copy, stdout_copy);
	return (status);
}

static void	close_heredocs(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == HEREDOC && redirs->fd >= 0)
		{
			close(redirs->fd);
			redirs->fd = -1;
		}
		redirs = redirs->next;
	}
}

/*
** Execute single command (no pipes)
** Builtins like cd/export/unset/exit run in parent
*/
int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_builtin(cmd->argv[0])
		&& is_parent_builtin(cmd->argv[0]))
	{
		shell->exit_status = exec_parent_builtin(cmd, shell);
		close_heredocs(cmd->redirs);
		return (shell->exit_status);
	}
	pid = execute_command(cmd, STDIN_FILENO,
			STDOUT_FILENO, shell);
	if (pid < 0)
	{
		shell->exit_status = 1;
		close_heredocs(cmd->redirs);
		return (1);
	}
	waitpid(pid, &status, 0);
	close_heredocs(cmd->redirs);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (0);
}

void	wait_last_pid(pid_t last_pid, t_shell *shell)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
}
