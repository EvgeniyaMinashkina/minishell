/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/09 14:31:57 by tkoval            #+#    #+#             */
/*   Updated: 2026/03/09 14:55:30 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_rparen(t_shell *shell, int *open)
{
	if (*open <= 0)
		return (throw_error(shell, ERR_UNCLOSED_PAREN));
	(*open)--;
	return (0);
}

static int	handle_lparen(t_shell *shell, t_token *curr, int *open)
{
	if (curr->next && is_binary_operator(curr->next))
		return (syntax_error_token(shell, token_type_to_str(curr->next)));
	(*open)++;
	return (0);
}

static int	invalid_before_rparen(t_shell *shell, t_token *curr)
{
	if (curr->next && curr->next->type == RPAREN
		&& (is_binary_operator(curr)
			|| is_redirection(curr)
			|| curr->type == LPAREN))
		return (syntax_error_token(shell, token_type_to_str(curr->next)));
	return (0);
}

int	check_parenthesis(t_shell *shell)
{
	int		open;
	t_token	*curr;

	curr = shell->tokens;
	open = 0;
	while (curr)
	{
		if (curr->type == RPAREN && handle_rparen(shell, &open))
			return (1);
		else if (curr->type == LPAREN && handle_lparen(shell, curr, &open))
			return (1);
		else if (invalid_before_rparen(shell, curr))
			return (1);
		curr = curr->next;
	}
	if (open > 0)
		return (throw_error(shell, ERR_UNCLOSED_PAREN));
	return (0);
}
