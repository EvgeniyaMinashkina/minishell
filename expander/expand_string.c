/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:08:14 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/21 20:48:39 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	single_quote_text_adding(char *str, int *i, char **res)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	append_str(res, ft_substr(str, start, *i - start));
}

static void	expand_var(char *str, int *i, t_shell *shell, char **res)
{
	char			*tmp;

	if (str[*i + 1] == '?')
	{
		tmp = ft_itoa(shell->exit_status);
		append_str(res, tmp);
		free(tmp);
		(*i) += 2;
		return ;
	}
	if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
	{
		(*i)++;
		append_var(str, i, shell, res);
		return ;
	}
	append_str(res, "$");
	(*i)++;
	return ;
}

static void	handle_quotes(char *str, int *i, t_quote_state	*quote, char **res)
{
	if (str[*i] == '\'' || str[*i] == '\"')
	{
		toggle_or_add_quote(quote, str[*i], res);
		(*i)++;
		return ;
	}
	if (*quote == SINGLE_QUOTE)
		single_quote_text_adding(str, i, res);
	return ;
}

char	*expand_string(char *str, t_shell *shell)
{
	int				i;
	char			*res;
	t_quote_state	quote;

	i = 0;
	res = ft_strdup("");
	quote = NONE;
	if (!str)
		return (NULL);
	while (str[i])
	{
		handle_quotes(str, &i, &quote, &res);
		if (str[i] == '$' && quote != SINGLE_QUOTE)
		{
			expand_var(str, &i, shell, &res);
			continue ;
		}
		append_str_norm(&res, &str[i], &i);
	}
	return (res);
}
