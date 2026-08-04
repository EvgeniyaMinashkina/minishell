/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:23:49 by tkoval            #+#    #+#             */
/*   Updated: 2026/08/04 15:09:27 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_output_fd(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
		return (pipefd[1]);
	return (STDOUT_FILENO);
}

static int	exec_pipeline_step(t_cmd *cmd, t_shell *shell,
	int *prev_fd, int pipefd[2])
{
	pid_t	pid;

	if (setup_pipe(cmd, pipefd))
		return (shell->exit_status = 1, 1);
	pid = execute_command(cmd, *prev_fd,
			get_output_fd(cmd, pipefd), shell);
	if (pid < 0)
	{
		shell->exit_status = 1;
		handle_exec_error(*prev_fd, cmd, pipefd);
		return (1);
	}
	update_fds(prev_fd, cmd, pipefd);
	return (pid);
}

static void	close_all_heredocs(t_cmd *cmd)
{
	t_redir	*r;

	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC && r->fd >= 0)
			{
				close(r->fd);
				r->fd = -1;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
}

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	last_pid;
	t_cmd	*cmd;

	prev_fd = STDIN_FILENO;
	cmd = cmd_list;
	last_pid = -1;
	while (cmd)
	{
		last_pid = exec_pipeline_step(cmd, shell, &prev_fd, pipefd);
		if (last_pid == 1)
			return (1);
		cmd = cmd->next;
	}
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	wait_last_pid(last_pid, shell);
	close_all_heredocs(cmd_list);
	return (0);
}
