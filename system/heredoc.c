/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:14:22 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/06 02:36:26 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	close(STDIN_FILENO);
}

static void	write_heredoc_line(char *line, int fd,
		bool expand, t_shell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_string(line, shell);
		if (expanded)
		{
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(fd, line, ft_strlen(line));

	write(fd, "\n", 1);
}

static int	heredoc_write(char *delim, bool expand,
		int write_fd, t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("> ");

		if (g_signal == SIGINT)
		{
			free(line);
			close(write_fd);
			return (130);
		}

		if (!line)
			break ;

		if (!ft_strncmp(line, delim, ft_strlen(delim) + 1))
		{
			free(line);
			break ;
		}

		write_heredoc_line(line, write_fd, expand, shell);
		free(line);
	}
	return (0);
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

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
		{
			close(read_fd);
			shell->exit_status = 130;
			return (-1);
		}
	}
	else if (WIFSIGNALED(status))
	{
		close(read_fd);
		shell->exit_status = 128 + WTERMSIG(status);
		return (-1);
	}

	return (read_fd);
}

static int	create_heredoc_pipe(t_cmd *cmd, t_shell *shell)
{
	int		fd[2];
	pid_t	pid;
	t_redir	*r;

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
	{
		close(fd[0]);

		g_signal = 0;
		signal(SIGINT, heredoc_sigint);

		r = cmd->redirs;
		while (r)
		{
			if (r->type == HEREDOC)
			{
				if (heredoc_write(r->filename,
						r->expand, fd[1], shell))
				{
					close(fd[1]);
					shell_exit(shell, 130);
				}
			}
			r = r->next;
		}

		close(fd[1]);
		shell_exit(shell, 0);
	}

	close(fd[1]);

	r = cmd->redirs;
	while (r)
	{
		if (r->type == HEREDOC)
			r->fd = fd[0];
		r = r->next;
	}

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
