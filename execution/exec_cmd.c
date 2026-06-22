/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:15:42 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/22 23:09:24 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	save_stdio(int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy < 0 || *stdout_copy < 0)
		return (1);
	return (0);
}

static void	restore_stdio(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}

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
		return (shell->exit_status);
	}
	pid = execute_command(cmd,
			STDIN_FILENO,
			STDOUT_FILENO,
			shell);
	if (pid < 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (0);
}

static void	wait_last_pid(pid_t last_pid, t_shell *shell)
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

static int	setup_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) < 0)
			return (1);
	}
	return (0);
}

static int	get_output_fd(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
		return (pipefd[1]);
	return (STDOUT_FILENO);
}

static int	handle_exec_error(int prev_fd, t_cmd *cmd, int pipefd[2])
{
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (cmd->next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	return (1);
}

static void	update_fds(int *prev_fd, t_cmd *cmd, int pipefd[2])
{
	if (*prev_fd != STDIN_FILENO)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

/*
** Execute commands connected by pipes
*/
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
		if (setup_pipe(cmd, pipefd))
		{
			shell->exit_status = 1;
			return (1);
		}
		last_pid = execute_command(cmd,
				prev_fd,
				get_output_fd(cmd, pipefd),
				shell);
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
}
