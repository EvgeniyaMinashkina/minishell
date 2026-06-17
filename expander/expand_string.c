/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 15:08:14 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/29 15:42:15 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	single_quote_text_adding(char *str, int *i, char **res)
{
	int start = *i;

	while (str[*i] && str[*i] != '\'')
		(*i)++;
	append_str(res, ft_substr(str, start, *i - start));
}


char	*expand_string(char *str, t_shell *shell)
{
	int				i;
	char			*res;
	t_quote_state	quote;
	char			*tmp;

	i = 0;
	res = ft_strdup("");
	quote = NONE;
	if (!str)
		return (NULL);
	while (str[i])
	{
		//Handle quotes
		if (str[i] == '\'' || str[i] == '\"')
		{
			toggle_or_add_quote(&quote, str[i], &res);
			i++;
			continue ;
		}
		if (quote == SINGLE_QUOTE)
		{
			single_quote_text_adding(str, &i, &res);
			continue;
		}
		//Handle variable expansion
		if (str[i] == '$' && quote != SINGLE_QUOTE)
		{
			i++;
			// case: end of string
			if (str[i] == '\0')
			{
				append_str(&res, "$");
				break ;
			}
			// case: $?
			if (str[i] == '?')
			{
				tmp = ft_itoa(shell->exit_status);
				append_str(&res, tmp);
				free(tmp);
				i++;
				continue;
			}
			// case: valid variable name
			if (ft_isalpha(str[i]) || str[i] == '_')
			{
				append_var(str, &i, shell, &res);
				continue;
			}
		}
		append_str_norm(&res, &str[i], &i);
	}
	return (res);
}
