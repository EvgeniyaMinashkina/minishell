/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:15:15 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 15:28:20 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static void	free_paths(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

static char	**get_paths_array(char **envp)
{
	char	*path_env;

	path_env = get_path_env(envp);
	if (!path_env)
		return (NULL);
	return (ft_split(path_env, ':'));
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_paths_array(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full = join_path(paths[i], cmd);
		if (full && access(full, X_OK) == 0)
		{
			free_paths(paths);
			return (full);
		}
		free(full);
		i++;
	}
	free_paths(paths);
	return (NULL);
}
