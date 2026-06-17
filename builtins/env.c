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

static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

char	**env_init(char **envp)
{
	char	**copy;
	int		i;
	int		size;

	size = env_count(envp);
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (--i >= 0)
				free(copy[i]);
			free(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

char	*env_get(char **envp, const char *key)
{
	int		i;
	size_t	len;

	if (!envp || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0
			&& envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*create_env_line(const char *key, const char *value)
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

int	env_set(char ***envp, const char *key, const char *value)
{
	char	**new_env;
	char	*new_line;
	int		i;
	int		size;

	if (!envp || !*envp || !key || !value)
		return (1);
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
	size = env_count(*envp);
	new_env = malloc(sizeof(char *) * (size + 2));
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
	{
		free(new_env);
		return (1);
	}
	new_env[i] = new_line;
	new_env[i + 1] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}

int	env_unset(char ***envp, const char *key)
{
	int	i;
	int	len;

	if (!envp || !*envp || !key)
		return (1);
	len = ft_strlen(key);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], key, len)
			&& (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			while ((*envp)[i + 1])
			{
				(*envp)[i] = (*envp)[i + 1];
				i++;
			}
			(*envp)[i] = NULL;
			return (0);
		}
		i++;
	}
	return (0);
}

void	free_env(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
