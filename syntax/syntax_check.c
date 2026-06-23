/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:24:54 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/23 18:35:30 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_binary_operator(t_token *token)
{
	if (token->type == PIPE)
		return (1);
	return (0);
}

int	is_redirection(t_token *token)
{
	if (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == APPEND || token->type == HEREDOC)
		return (1);
	return (0);
}

//syntax_check
//check all parenthesis, if any is enclosed
//check if first is bynary operator (which requires WORD from both sides)
//in while
//	check two operators
//	check redirection without file
//	check the last token
int	syntax_check(t_shell *shell)
{
	t_token	*curr;

	curr = shell->tokens;
	if (!curr)
		return (0);
	if (is_binary_operator(curr))
		return (syntax_error_token(shell, token_type_to_str(curr)));
	while (curr)
	{
		if (curr->next && is_binary_operator(curr)
			&& is_binary_operator(curr->next))
			return (syntax_error_token(shell, token_type_to_str(curr->next)));
		if (is_redirection(curr) && (!curr->next || curr->next->type != WORD))
			return (syntax_error_token(shell, token_type_to_str(curr->next)));
		if (!curr->next && is_binary_operator(curr))
			return (syntax_error_token(shell, token_type_to_str(curr->next)));
		curr = curr->next;
	}
	return (0);
}
