/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:14:22 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/06 03:13:33 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_heredoc_status(int status, int read_fd, t_shell *shell)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(read_fd);
		shell->exit_status = 130;
		return (-1);
	}
	if (WIFSIGNALED(status))
	{
		close(read_fd);
		shell->exit_status = 128 + WTERMSIG(status);
		return (-1);
	}
	return (read_fd);
}

static int	wait_heredoc(pid_t pid, int read_fd, t_shell *shell)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
		{
			close(read_fd);
			init_signals_prompt();
			return (-1);
		}
	}
	init_signals_prompt();
	return (check_heredoc_status(status, read_fd, shell));
}

static void	assign_heredoc_fd(t_redir *redirs, int fd)
{
	while (redirs)
	{
		if (redirs->type == HEREDOC)
			redirs->fd = fd;
		redirs = redirs->next;
	}
}

static int	create_heredoc_pipe(t_cmd *cmd, t_shell *shell)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		init_signals_prompt();
		return (1);
	}
	if (pid == 0)
		heredoc_child(cmd, fd, shell);
	close(fd[1]);
	assign_heredoc_fd(cmd->redirs, fd[0]);
	return (wait_heredoc(pid, fd[0], shell));
}

int	prepare_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmds;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				if (create_heredoc_pipe(cmd, shell) < 0)
					return (1);
				break ;
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
