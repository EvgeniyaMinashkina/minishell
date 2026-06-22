/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:16:12 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/22 23:11:14 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_exit(t_shell *shell, int status)
{
	shell_exit(shell, status);
}

#include <sys/stat.h> //TODO
#include <errno.h>

static void	print_exec_error(char *cmd)
{
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
}

static int	is_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

int	execute_command(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell)
{
	pid_t		pid;
	char		*path;
	struct stat	st;

	pid = fork();
	if (pid < 0)
		return (-1);

	if (pid == 0)
	{
		init_signals_child();

		if (!cmd || !cmd->argv || !cmd->argv[0])
			child_exit(shell, 0);

		// -------------------------
		// 1. INPUT / OUTPUT SETUP
		// -------------------------
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) < 0)
				child_exit(shell, 1);
			close(in_fd);
		}

		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) < 0)
				child_exit(shell, 1);
			close(out_fd);
		}

		// 🔥 CRITICAL FIX #1:
		// закрываем ВСЕ лишние pipe fd, которые могли унаследоваться
		// иначе yes | head висит
		for (int fd = 3; fd < 1024; fd++)
			close(fd);

		if (apply_redirections(cmd->redirs, shell))
			child_exit(shell, shell->exit_status);

		// -------------------------
		// 2. BUILTINS (child side)
		// -------------------------
		if (is_builtin(cmd->argv[0]) && !is_parent_builtin(cmd->argv[0]))
			child_exit(shell, exec_builtin(cmd, shell));

		// -------------------------
		// 3. PATH RESOLUTION
		// -------------------------
		if (is_path(cmd->argv[0]))
		{
			// DIRECT PATH (./file, /bin/ls)
			if (stat(cmd->argv[0], &st) != 0)
			{
				print_exec_error(cmd->argv[0]);
				ft_putendl_fd("No such file or directory", 2);
				child_exit(shell, 127);
			}

			if (S_ISDIR(st.st_mode))
			{
				print_exec_error(cmd->argv[0]);
				ft_putendl_fd("is a directory", 2);
				child_exit(shell, 126);
			}

			if (access(cmd->argv[0], X_OK) == -1)
			{
				print_exec_error(cmd->argv[0]);
				ft_putendl_fd("Permission denied", 2);
				child_exit(shell, 126);
			}

			path = ft_strdup(cmd->argv[0]);
		}
		else
		{
			// PATH SEARCH
			path = find_cmd_path(cmd->argv[0], shell->envp);
			if (!path)
			{
				print_exec_error(cmd->argv[0]);
				ft_putendl_fd("command not found", 2);
				child_exit(shell, 127);
			}
		}

		// -------------------------
		// 4. EXECUTION
		// -------------------------
		execve(path, cmd->argv, shell->envp);

		// only if execve fails
		print_exec_error(cmd->argv[0]);

		if (errno == EACCES)
			ft_putendl_fd("Permission denied", 2);
		else if (errno == EISDIR)
			ft_putendl_fd("is a directory", 2);
		else
			perror("execve");

		free(path);
		child_exit(shell, 126);
	}

	return (pid);
}

/*
** Fork + execute one command
** Handles:
** - redirections
** - builtins inside pipeline
** - execve for external commands
*/
/*int	execute_command(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		init_signals_child();
		if (!cmd || !cmd->argv || !cmd->argv[0])
			child_exit(shell, 0);
		if (in_fd != STDIN_FILENO)
		{
			if (dup2(in_fd, STDIN_FILENO) < 0)
				child_exit(shell, 1);
			close(in_fd);
		}
		if (out_fd != STDOUT_FILENO)
		{
			if (dup2(out_fd, STDOUT_FILENO) < 0)
				child_exit(shell, 1);
			close(out_fd);
		}
		if (apply_redirections(cmd->redirs, shell))
			child_exit(shell, shell->exit_status);
		if (is_builtin(cmd->argv[0]))
			child_exit(shell, exec_builtin(cmd, shell));
		path = find_cmd_path(cmd->argv[0], shell->envp);
		if (!path)
		{
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			child_exit(shell, 127);
		}
		execve(path, cmd->argv, shell->envp);
		perror("execve");
		free(path);
		child_exit(shell, 126);
	}
	return (pid);
}*/