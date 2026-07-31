/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:16:12 by yminashk          #+#    #+#             */
/*   Updated: 2026/07/31 23:45:46 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
execute_pipeline
   └── execute_command
         └── execute_child
               ├── input/output
               ├── redirections
               ├── builtin
               └── exec
*/

static void	input_output_setup(t_shell *shell, int *in_fd, int *out_fd)
{
	if (*in_fd != STDIN_FILENO)
	{
		if (dup2(*in_fd, STDIN_FILENO) < 0)
			shell_exit(shell, 1);
		close(*in_fd);
	}
	if (*out_fd != STDOUT_FILENO)
	{
		if (dup2(*out_fd, STDOUT_FILENO) < 0)
			shell_exit(shell, 1);
		close(*out_fd);
	}
}

// CRITICAL FIX #1:
// закрываем ВСЕ лишние pipe fd, которые могли унаследоваться
// иначе yes | head висит*/
void	close_odd_pipes(void)
{
	int	fd;

	fd = 3;
	while (fd < 1024)
	{
		close(fd);
		fd++;
	}
}

// struct stat	st;
static void	exec_process(t_cmd *cmd, char *path, t_shell *shell)
{
	execve(path, cmd->argv, shell->envp);
	print_exec_error(cmd->argv[0]);
	if (errno == EACCES)
		ft_putendl_fd("Permission denied", 2);
	else if (errno == EISDIR)
		ft_putendl_fd("is a directory", 2);
	else
		perror("execve");
	free(path);
	shell_exit(shell, 126);
}

static void	execute_child(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell)
{
	char	*path;

	init_signals_child();
	if (!cmd || !cmd->argv || !cmd->argv[0])
		shell_exit(shell, 0);
	input_output_setup(shell, &in_fd, &out_fd);
	close_odd_pipes();
	if (apply_redirections(cmd->redirs, shell))
		shell_exit(shell, shell->exit_status);
	if (is_builtin(cmd->argv[0]))
		shell_exit(shell, exec_builtin(cmd, shell));
	path = resolve_path(cmd, shell);
	if (!path)
	{
		print_exec_error(cmd->argv[0]);
		if (errno == EACCES)
			ft_putendl_fd("Permission denied", 2);
		else if (errno == EISDIR)
			ft_putendl_fd("Is a directory", 2);
		else if (ft_strchr(cmd->argv[0], '/'))
			ft_putendl_fd("No such file or directory", 2);
		else
			ft_putendl_fd("command not found", 2);
		shell_exit(shell,
			errno == EACCES || errno == EISDIR ? 126 : 127);
	}
	exec_process(cmd, path, shell);
}

int	execute_command(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
		execute_child(cmd, in_fd, out_fd, shell);
	return (pid);
}
