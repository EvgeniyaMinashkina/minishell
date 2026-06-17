/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 15:11:29 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/29 13:45:57 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->next = NULL;
	cmd->redirs = NULL;
	return (cmd);
}

static t_redir	*new_redir(t_token_type type, char *filename)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->filename = ft_strdup(filename);
	new->type = type;
	new->next = NULL;
	return (new);
}

void	add_redir(t_cmd *cmd, t_token_type type, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	if (!cmd)
		return ;
	new = new_redir(type, filename);
	if (!new)
		return ;
	if (cmd->redirs == NULL)
	{
		cmd->redirs = new;
		return ;
	}
	tmp = cmd->redirs;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	add_argv(t_cmd *cmd, char *value)
{
	char	**new_argv;
	int		i;

	if (!cmd || !value)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
		i++;
	new_argv = malloc(sizeof(char *) * (i + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i] = ft_strdup(value);
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void	add_cmd_back(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	if (!cmd_list || !new_cmd)
		return ;
	if (*cmd_list == NULL)
	{
		*cmd_list = new_cmd;
		return ;
	}
	tmp = *cmd_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_cmd;
}
