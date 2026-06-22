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
