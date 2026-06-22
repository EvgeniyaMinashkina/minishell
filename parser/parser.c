/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:39:42 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/22 23:40:21 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_error(t_shell *shell, t_error_type type, t_cmd **cmd_list)
{
	throw_error(shell, type);
	if (cmd_list && *cmd_list)
		free_cmds(*cmd_list);
	free_tokens(shell->tokens);
	shell->tokens = NULL;
}

static int	handle_token(t_shell *shell, t_cmd *cmd,
				t_token **head, t_cmd *cmd_list)
{
	t_token_type	redir_type;

	if ((*head)->type == WORD)
		add_argv(cmd, (*head)->value);
	else if ((*head)->type == REDIR_IN || (*head)->type == REDIR_OUT
		|| (*head)->type == APPEND || (*head)->type == HEREDOC)
	{
		redir_type = (*head)->type;
		*head = (*head)->next;
		if (!*head || (*head)->type != WORD)
			return (handle_error(shell, ERR_GENERAL, &cmd_list), 0);
		add_redir(cmd, redir_type, (*head)->value);
	}
	return (1);
}

static t_cmd	*build_cmd_block(t_shell *shell, t_token **head,
						t_cmd **cmd_list)
{
	t_cmd	*cmd;

	cmd = new_cmd();
	if (!cmd)
		return (handle_error(shell, ERR_MALLOC, cmd_list), NULL);
	add_cmd_back(cmd_list, cmd);
	while (*head && (*head)->type != PIPE)
	{
		if (!handle_token(shell, cmd, head, *cmd_list))
			return (NULL);
		*head = (*head)->next;
	}
	return (cmd);
}

t_cmd	*parse_tokens(t_shell *shell)
{
	t_token	*head;
	t_cmd	*curr_cmd;
	t_cmd	*cmd_list;

	head = shell->tokens;
	cmd_list = NULL;
	curr_cmd = NULL;
	while (head)
	{
		if (!curr_cmd)
		{
			curr_cmd = build_cmd_block(shell, &head, &cmd_list);
			if (!curr_cmd)
				return (NULL);
		}
		if (head && head->type == PIPE)
		{
			curr_cmd = NULL;
			head = head->next;
		}
	}
	return (cmd_list);
}
