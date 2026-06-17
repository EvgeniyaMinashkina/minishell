/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_search.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 16:52:13 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/29 15:27:41 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable_name(char *str)
{
	char	*res;
	int		len;

	len = 0;
	while (str[len] && is_valid_char(str[len]))
		len++;
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	len = 0;
	while (str[len] && is_valid_char(str[len]))
	{
		res[len] = str[len];
		len++;
	}
	res[len] = '\0';
	return (res);
}

char	*get_env_value(char **envp, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	if (!name || !envp)
		return (NULL);
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
