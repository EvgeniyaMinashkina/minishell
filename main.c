/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 13:12:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/18 21:35:13 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	process_input(t_shell *shell, char *line)
{
	shell->tokens = lexer(shell, line);
	if (!shell->tokens)
		return ;
	syntax_check(shell);
	if (shell->syntax_error_msg)
	{
		ft_putendl_fd(shell->syntax_error_msg, 2);
		free(shell->syntax_error_msg);
		shell->syntax_error_msg = NULL;
		free_tokens(shell->tokens);
		return ;
	}
	shell->cmd_list = parse_tokens(shell);
	/*TODO: Проверку ниже можно будет удалить, когда будет обработана последняя ошибка в парсере*/
	if (!shell->cmd_list)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	print_cmds(shell->cmd_list);
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

void new_shell(t_shell *shell, char **envp)
{
	shell->envp = env_init(envp);
	shell->syntax_error_msg = NULL;
	shell->exit_status = 0;
	shell->cmd_list = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	new_shell(&shell, envp);
	init_signals_prompt();
	while (1)
	{
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
		{
			shell.exit_status = 130;
			g_signal = 0;
		}
		if (!line)
		{
			printf("exit\n");
			shell_exit(&shell, shell.exit_status);
		}
		if (*line)
			add_history(line);
		process_input(&shell, line);
		free(line);
	}
}
