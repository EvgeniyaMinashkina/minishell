/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:23:49 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/23 00:28:28 by tkoval           ###   ########.fr       */
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
	return (0);
}

/*
** Execute commands connected by pipes
*/
/*int	execute_pipeline(t_cmd *cmd_list, t_shell *shell) //TODO
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
		if (setup_pipe(cmd, pipefd))
		{
			shell->exit_status = 1;
			return (1);
		}
		last_pid = execute_command(cmd, prev_fd,
				get_output_fd(cmd, pipefd), shell);
		if (last_pid < 0)
		{
			shell->exit_status = 1;
			return (handle_exec_error(prev_fd, cmd, pipefd));
		}
		update_fds(&prev_fd, cmd, pipefd);
		cmd = cmd->next;
	}
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	wait_last_pid(last_pid, shell);
	return (0);
}*/
