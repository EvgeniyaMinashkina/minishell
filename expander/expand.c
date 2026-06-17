/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:59:44 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/29 15:26:05 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_redirs(t_cmd *cmd, t_shell *shell)
{
	t_redir	*tmp;

	tmp = cmd->redirs;
	while (tmp)
	{
		tmp->filename = expand_string(tmp->filename, shell);
		tmp = tmp->next;
	}
}

void	expand_cmds(t_shell *shell)
{
	t_cmd	*tmp;
	int		i;

	tmp = shell->cmd_list;
	while (tmp)
	{
		i = 0;
		while (tmp->argv[i])
		{
			tmp->argv[i] = expand_string(tmp->argv[i], shell);
			i++;
		}
		if (tmp->redirs)
			expand_redirs(tmp, shell);
		tmp = tmp->next;
	}
}
