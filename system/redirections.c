/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 14:50:34 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/12 14:18:15 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Open file depending on redirection type
*/
static int	open_file(t_redir *r)
{
	if (r->type == REDIR_IN)
		return (open(r->filename, O_RDONLY));
	else if (r->type == REDIR_OUT)
	{
		return (open(r->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644));
	}
	else if (r->type == APPEND)
	{
		return (open(r->filename,
				O_WRONLY | O_CREAT | O_APPEND, 0644));
	}
	return (-1);
}

static int	redirect_fd(int fd, int target, t_shell *shell)
{
	if (dup2(fd, target) < 0)
	{
		close(fd);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	get_redir_fd(t_redir *r, t_shell *shell,
				int *fd, int *target)
{
	if (r->type == HEREDOC)
	{
		*fd = heredoc_pipe(r->filename, shell);
		*target = STDIN_FILENO;
		if (*fd < 0)
		{
			shell->exit_status = 1;
			return (1);
		}
	}
	else
	{
		*fd = open_file(r);
		if (*fd < 0)
		{
			perror(r->filename);
			shell->exit_status = 1;
			return (1);
		}
		if (r->type == REDIR_IN)
			*target = STDIN_FILENO;
		else
			*target = STDOUT_FILENO;
	}
	return (0);
}

/*
** Apply all redirections in order
*/
int	apply_redirections(t_redir *redirs, t_shell *shell)
{
	t_redir	*r;
	int		fd;
	int		target;

	r = redirs;
	while (r)
	{
		if (get_redir_fd(r, shell, &fd, &target))
			return (1);
		if (redirect_fd(fd, target, shell))
			return (1);
		close(fd);
		r = r->next;
	}
	return (0);
}
