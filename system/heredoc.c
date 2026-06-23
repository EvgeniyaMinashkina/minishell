/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:14:22 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 13:44:26 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(char *delim, int write_fd, t_shell *shell)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delim,
				ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		expanded = expand_string(line, shell);
		write(write_fd, expanded, ft_strlen(expanded));
		write(write_fd, "\n", 1);
		free(expanded);
		free(line);
	}
	close(write_fd);
	exit(0);
}

int	heredoc_pipe(char *delim, t_shell *shell)
{
	int		fd[2];
	pid_t	pid;
	int		status;

	if (pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		close(fd[0]);
		heredoc_child(delim, fd[1], shell);
	}
	close(fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		close(fd[0]);
		shell->exit_status = 130;
		return (-1);
	}
	return (fd[0]);
}
