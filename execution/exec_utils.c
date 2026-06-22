/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/23 00:13:19 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/23 00:21:50 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) < 0)
			return (1);
	}
	return (0);
}

int	handle_exec_error(int prev_fd, t_cmd *cmd, int pipefd[2])
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

void	update_fds(int *prev_fd, t_cmd *cmd, int pipefd[2])
{
	if (*prev_fd != STDIN_FILENO)
		close(*prev_fd);
	if (cmd->next)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

int	save_stdio(int *stdin_copy, int *stdout_copy)
{
	*stdin_copy = dup(STDIN_FILENO);
	*stdout_copy = dup(STDOUT_FILENO);
	if (*stdin_copy < 0 || *stdout_copy < 0)
		return (1);
	return (0);
}

void	restore_stdio(int stdin_copy, int stdout_copy)
{
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
