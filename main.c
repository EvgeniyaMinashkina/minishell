/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 13:12:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/18 18:15:22 by tkoval           ###   ########.fr       */
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

/*
void new_shell(t_shell *shell, char **envp)
{
	shell->envp = envp;
	shell->syntax_error_msg = NULL;
	shell->exit_status = 0;
	shell->cmd_list = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	t_shell	shell;

	if (argc != 1 || !argv[0])
		return (ft_printf("minishell: invalid arguments\n"), 2);
	new_shell(&shell, envp);
	while (1)
	{
		line = readline("minishell> ");
		if (!line) //Ctrl-D
		{
			ft_printf("exit\n");
			break;
		}
		if (*line == '\0')
		{
			shell.exit_status = 0;
			// printf ("Exit status %d\n", shell.exit_status);
			free(line);
			continue;
		}
		if (*line)
			add_history(line);
		shell.tokens = lexer(&shell, line);
		syntax_check(&shell);
		if (shell.syntax_error_msg)
		{
			ft_putendl_fd(shell.syntax_error_msg, 2);
			free(shell.syntax_error_msg);
			shell.syntax_error_msg = NULL;
		}
		else
		{
			// printf ("Exit status %d\n", shell.exit_status);
			shell.cmd_list = parse_tokens(&shell);
			expand_cmds(&shell);
			print_cmds(shell.cmd_list);
			printf("------------------------------------------\n");
		}
		free(line);
		free_tokens(shell.tokens);
	}

	return (0);
}
*/
