/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 12:58:57 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/22 23:20:18 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_word(t_shell *shell, char *str, int *i)
{
	t_quote_state	state;
	char			*result;
	char			buffer[10000];
	int				j;

	state = NONE;
	j = 0;
	while (str[*i])
	{
		if (state == NONE && (is_space(str[*i]) || is_operator(&str[*i])))
			break ;
		if ((str[*i] == '\'' && state != DOUBLE_QUOTE)
			|| (str[*i] == '\"' && state != SINGLE_QUOTE))
			toggle_quote_state(&state, str[*i]);
		buffer[j++] = str[*i];
		(*i)++;
	}
	if (state != NONE && !shell->error_msg)
		return (throw_error(shell, ERR_UNCLOSED_QUOTE), NULL);
	if (state != NONE)
		return (NULL);
	buffer[j] = '\0';
	result = ft_strdup(buffer);
	return (result);
}

t_token	*add_new_token(t_token **head, t_token_type type, char *value,
						t_quote_state state)
{
	t_token	*new_token;
	t_token	*last;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = value;
	new_token->quote_state = state;
	new_token->next = NULL;
	if (*head == NULL)
		*head = new_token;
	else
	{
		last = *head;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
	return (new_token);
}

t_token	*add_operator_token(t_token **head, char *str, t_quote_state state)
{
	t_token	*new_token;

	new_token = NULL;
	if (!str || !*str)
		return (0);
	if (!ft_strncmp(str, "<<", 2))
		new_token = add_new_token(head, HEREDOC, NULL, state);
	else if (!ft_strncmp(str, ">>", 2))
		new_token = add_new_token(head, APPEND, NULL, state);
	else if (!ft_strncmp(str, "&&", 2))
		new_token = add_new_token(head, AND, NULL, state);
	else if (!ft_strncmp(str, "||", 2))
		new_token = add_new_token(head, OR, NULL, state);
	else if (*str == '|')
		new_token = add_new_token(head, PIPE, NULL, state);
	else if (*str == '<')
		new_token = add_new_token(head, REDIR_IN, NULL, state);
	else if (*str == '>')
		new_token = add_new_token(head, REDIR_OUT, NULL, state);
	else if (*str == '(')
		new_token = add_new_token(head, LPAREN, NULL, state);
	else if (*str == ')')
		new_token = add_new_token(head, RPAREN, NULL, state);
	return (new_token);
}

t_token	*lexer(t_shell *shell, char *str)
{
	int			i;
	t_token		*head;
	char		*curr_word;

	i = 0;
	head = NULL;
	while (str[i])
	{
		if (is_space(str[i]))
			i++;
		else if (is_operator(&str[i]))
		{
			add_operator_token(&head, &str[i], NONE);
			i += is_operator(&str[i]);
		}
		else
		{
			curr_word = build_word(shell, str, &i);
			if (!curr_word || shell->error_msg)
				return (free_tokens(head), NULL);
			add_new_token(&head, WORD, curr_word, NONE);
		}
	}
	return (head);
}
