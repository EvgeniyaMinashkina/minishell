/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:13:26 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 16:16:15 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_check_args(char **argv)
{
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	return (0);
}

static char	*cd_get_path(char **argv, char **envp)
{
	char	*path;

	if (argv[1] && !ft_strncmp(argv[1], "-", 2))
	{
		path = env_get(envp, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("cd: OLDPWD not set", 2);
			return (NULL);
		}
		printf("%s\n", path);
		return (path);
	}
	if (!argv[1])
	{
		path = env_get(envp, "HOME");
		if (!path)
			ft_putendl_fd("cd: HOME not set", 2);
		return (path);
	}
	return (argv[1]);
}

static void	cd_update_env(char **envp, char *oldpwd, char *cwd)
{
	if (oldpwd)
		env_set(&envp, "OLDPWD", oldpwd);
	if (getcwd(cwd, 4096))
		env_set(&envp, "PWD", cwd);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*oldpwd;
	char	*path;
	char	cwd[4096];

	if (cd_check_args(argv))
		return (1);
	oldpwd = env_get(shell->envp, "PWD");
	path = cd_get_path(argv, shell->envp);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	cd_update_env(shell->envp, oldpwd, cwd);
	return (0);
}
