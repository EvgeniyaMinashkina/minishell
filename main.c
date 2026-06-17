/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 13:12:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/12 14:18:27 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	process_input(t_shell *shell, char *line)
{
	shell->tokens = lexer(shell, line);
	if (!shell->tokens)
		return ;
	if (syntax_check(shell))
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
	shell->cmd_list = parse_tokens(shell);
	if (!shell->cmd_list)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		return ;
	}
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

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*line;
	char	*trim;

	(void)argc;
	(void)argv;
	ft_memset(&shell, 0, sizeof(t_shell));
	shell.envp = env_init(envp);
	shell.exit_status = 0;
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
		trim = ft_strtrim(line, " \t");
		if (!trim || !*trim)
		{
			free(trim);
			free(line);
			continue ;
		}
		free(trim);
		add_history(line);
		process_input(&shell, line);
		free(line);
	}
}
