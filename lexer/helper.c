/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:04:04 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/23 18:38:24 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int	is_operator(char *str)
{
	if (!str || !*str)
		return (0);
	if (!ft_strncmp(str, "<<", 2))
		return (2);
	if (!ft_strncmp(str, ">>", 2))
		return (2);
	if (*str == '|' || *str == '<' || *str == '>')
		return (1);
	return (0);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	toggle_quote_state(t_quote_state *state, char c)
{
	if (c == '\'' && *state != DOUBLE_QUOTE)
	{
		if (*state == NONE)
			*state = SINGLE_QUOTE;
		else
			*state = NONE;
	}
	if (c == '\"' && *state != SINGLE_QUOTE)
	{
		if (*state == NONE)
			*state = DOUBLE_QUOTE;
		else
			*state = NONE;
	}
}
