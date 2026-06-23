/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 13:12:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 16:18:20 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	print_error(t_shell *shell)
{
	ft_putendl_fd(shell->error_msg, 2);
	free(shell->error_msg);
	shell->error_msg = NULL;
	free_tokens(shell->tokens);
	return ;
}

static void	process_input(t_shell *shell, char *line)
{
	shell->tokens = lexer(shell, line);
	if (shell->error_msg)
		return (print_error(shell));
	syntax_check(shell);
	if (shell->error_msg)
		return (print_error(shell));
	shell->cmd_list = parse_tokens(shell);
	if (shell->error_msg)
		return (print_error(shell));
	if (!shell->cmd_list)
		return ;
	expand_cmds(shell);
	if (!shell->cmd_list->next)
		execute_single_command(shell->cmd_list, shell);
	else
		execute_pipeline(shell->cmd_list, shell);
	free_tokens(shell->tokens);
	free_cmds(shell->cmd_list);
	shell->tokens = NULL;
	shell->cmd_list = NULL;
}

static void	new_shell(t_shell *shell, char **envp)
{
	shell->envp = env_init(envp);
	shell->error_msg = NULL;
	shell->exit_status = 0;
	shell->cmd_list = NULL;
}

static void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
		if (!line)
		{
			printf("exit\n");
			shell_exit(shell, shell->exit_status);
		}
		if (*line)
			add_history(line);
		process_input(shell, line);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	new_shell(&shell, envp);
	init_signals_prompt();
	shell_loop(&shell);
}
