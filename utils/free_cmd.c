/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:47:19 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/12 14:18:24 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs)
	{
		tmp = redirs->next;
		free(redirs->filename);
		free(redirs);
		redirs = tmp;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->argv)
		{
			i = 0;
			while (cmds->argv[i])
			{
				free(cmds->argv[i]);
				i++;
			}
			free(cmds->argv);
		}
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}

void	shell_exit(t_shell *shell, int status)
{
	if (shell)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_cmds(shell->cmd_list);
		shell->cmd_list = NULL;
		free_env(shell->envp);
		shell->envp = NULL;
	}
	rl_clear_history();
	exit(status);
}
