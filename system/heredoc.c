/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:14:22 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/03 13:34:01 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	close(STDIN_FILENO);
}

static int	wait_heredoc(pid_t pid, int read_fd, t_shell *shell)
{
	int	status;

	while (waitpid(pid, &status, 0) == -1)
	{
		if (errno != EINTR)
		{
			close(read_fd);
			return (-1);
		}
	}
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

static void	write_heredoc_line(char *line, int write_fd,
	bool expand, t_shell *shell)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_string(line, shell);
		write(write_fd, expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
}

static void	heredoc_child(char *delim, int write_fd,
	bool expand, t_shell *shell)
{
	char	*line;

	g_signal = 0;
	signal(SIGINT, heredoc_sigint);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			close(write_fd);
			shell_exit(shell, 130);
		}
		if (!line)
			break ;
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(line, write_fd, expand, shell);
		free(line);
	}
	close(write_fd);
	shell_exit(shell, 0);
}

int	heredoc_pipe(char *delim, bool expand, t_shell *shell)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(delim, fd[1], expand, shell);
	}
	close(fd[1]);
	return (wait_heredoc(pid, fd[0], shell));
}
