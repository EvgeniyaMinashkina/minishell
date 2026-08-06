/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:14:22 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/06 03:09:18 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	heredoc_child(t_cmd *cmd, int *fd, t_shell *shell)
{
	t_redir	*r;

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
