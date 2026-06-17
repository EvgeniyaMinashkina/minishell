/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 14:39:42 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/20 15:12:28 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse_tokens(t_shell *shell)
{
	t_token			*head;
	t_cmd			*curr_cmd;
	t_cmd			*cmd_list;
	t_token_type	redir_type;

	head = shell->tokens;
	cmd_list = NULL;
	curr_cmd = NULL;
	while (head)
	{
		if (curr_cmd == NULL)
		{
			curr_cmd = new_cmd();
			if (!curr_cmd)
				return (NULL); //TODO: handle error
			add_cmd_back(&cmd_list, curr_cmd);
		}
		while (head && head->type != PIPE)
		{
			if (head->type == WORD)
				add_argv(curr_cmd, head->value);
			else if (head->type == REDIR_IN || head->type == REDIR_OUT
				|| head->type == APPEND || head->type == HEREDOC)
			{
				redir_type = head->type;
				head = head->next;
				if (head == NULL || head->type != WORD)
					throw_error(shell, ERR_EXPECTED_FILENAME);
				add_redir(curr_cmd, redir_type, head->value);
			}
			head = head->next;
		}
		if (head && head->type == PIPE)
		{
			curr_cmd = NULL;
			head = head->next;
		}
	}
	return (cmd_list);
}
