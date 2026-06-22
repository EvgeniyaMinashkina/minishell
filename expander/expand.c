/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 14:59:44 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/22 23:15:27 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_redirs(t_cmd *cmd, t_shell *shell)
{
	t_redir	*tmp;
	char	*new_filename;

	tmp = cmd->redirs;
	while (tmp)
	{
		new_filename = expand_string(tmp->filename, shell);
		free(tmp->filename);
		tmp->filename = new_filename;
		tmp = tmp->next;
	}
}

static void	clean_empty_args(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] != '\0')
			cmd->argv[j++] = cmd->argv[i];
		else
			free(cmd->argv[i]);
		i++;
	}
	cmd->argv[j] = NULL;
}

void	expand_cmds(t_shell *shell)
{
	t_cmd	*tmp;
	char	*new_str;
	int		i;

	tmp = shell->cmd_list;
	while (tmp)
	{
		i = 0;
		while (tmp->argv[i])
		{
			new_str = expand_string(tmp->argv[i], shell);
			free(tmp->argv[i]);
			tmp->argv[i] = new_str;
			i++;
		}
		if (tmp->redirs)
			expand_redirs(tmp, shell);
		tmp = tmp->next;
	}
	clean_empty_args(shell->cmd_list);
}
