/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 16:54:50 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/29 15:36:18 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_char(char c)
{
	if (ft_isalpha(c) || ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

void	append_str(char **res, char *str)
{
	char	*new_res;

	new_res = ft_strljoin(*res, str, ft_strlen(str));
	free(*res);
	*res = new_res;
}

void	append_str_norm(char **res, char *str, int *i)
{
	char	*new_res;
	int		len;

	len = 0;
	while (str[len] && str[len] != '\'' && str[len] != '\"' && str[len] != '$')
		len++;
	new_res = ft_strljoin(*res, str, len);
	free(*res);
	*res = new_res;
	*i += len;
}

void	append_var(char *str, int *i, t_shell *shell, char **res)
{
	char	*env_val;
	char	*var_name;

	var_name = get_variable_name(&str[*i]);
	env_val = get_env_value(shell->envp, var_name);
	if (env_val == NULL)
		env_val = "";
	append_str(res, env_val);
	*i += ft_strlen(var_name);
	free(var_name);
}

void	toggle_or_add_quote(t_quote_state *quote, char c, char **res)
{
	if (c == '\'' && *quote != DOUBLE_QUOTE)
	{
		if (*quote == SINGLE_QUOTE)
			*quote = NONE;
		else
			*quote = SINGLE_QUOTE;
	}
	else if (c == '\'' && *quote == DOUBLE_QUOTE)
		append_str(res, "\'");
	if (c == '\"' && *quote != SINGLE_QUOTE)
	{
		if (*quote == DOUBLE_QUOTE)
			*quote = NONE;
		else
			*quote = DOUBLE_QUOTE;
	}
	else if (c == '\"' && *quote == SINGLE_QUOTE)
		append_str(res, "\"");
}
