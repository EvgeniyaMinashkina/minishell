/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:13:51 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/12 14:17:33 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	*create_env_line(const char *key, const char *value)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, value);
	free(tmp);
	return (res);
}

static int	update_env_var(char ***envp, const char *key, const char *value)
{
	int		i;
	char	*new_line;

	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, ft_strlen(key))
			&& (*envp)[i][ft_strlen(key)] == '=')
		{
			new_line = create_env_line(key, value);
			if (!new_line)
				return (1);
			free((*envp)[i]);
			(*envp)[i] = new_line;
			return (0);
		}
		i++;
	}
	return (-1);
}

static int	add_env_var(char ***envp, const char *key, const char *value)
{
	char	**new_env;
	char	*new_line;
	int		i;

	i = env_count(*envp);
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = 0;
	while ((*envp)[i])
	{
		new_env[i] = (*envp)[i];
		i++;
	}
	new_line = create_env_line(key, value);
	if (!new_line)
		return (free(new_env), 1);
	new_env[i] = new_line;
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

int	env_set(char ***envp, const char *key, const char *value)
{
	int	status;

	if (!envp || !*envp || !key || !value)
		return (1);
	status = update_env_var(envp, key, value);
	if (status != -1)
		return (status);
	return (add_env_var(envp, key, value));
}
