/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 13:12:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/24 17:26:42 by tkoval           ###   ########.fr       */
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

int	unclosed_quote(char *str)
{
	t_quote_state	q;
	int				i;

	i = 0;
	q = NONE;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' && q == NONE)
			q = SINGLE_QUOTE;
		else if (str[i] == '\'' && q == SINGLE_QUOTE)
			q = NONE;
		else if (str[i] == '"' && q == NONE)
			q = DOUBLE_QUOTE;
		else if (str[i] == '"' && q == DOUBLE_QUOTE)
			q = NONE;
		i++;
	}
	return (q != NONE);
}

static void	sigint_dquote(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
}

char	*append_line(char *line)
{
	char	*tmp;
	char	*new;

	while (unclosed_quote(line))
	{
		tmp = readline("dquote> ");
		if (!tmp)
		{
			free(line);
			return (NULL);
		}
		new = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(new, tmp);
		free(new);
		free(tmp);
	}
	return (line);
}

/*
char	*read_multiline(char *line)
{
	char	*tmp;
	char	*new;

	while (unclosed_quote(line))
	{
		signal(SIGINT, sigint_dquote);
		tmp = readline("dquote> ");
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			free(tmp);
			free(line);
			init_signals_prompt();
			return (NULL);
		}
		if (!tmp)
			break ;
		new = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(new, tmp);
		free(new);
		free(tmp);
	}
	init_signals_prompt();
	return (line);
}*/

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
		rl_event_hook = NULL;
		line = readline("minishell$ ");

		if (!line)
		{
			printf("exit\n");
			shell_exit(shell, shell->exit_status);
		}
		if (unclosed_quote(line))
			line = append_line(line);
		if (!line)
		{
			// rl_replace_line("", 0);
			// rl_on_new_line();
			g_signal = 0;
			// rl_on_new_line();
			shell->exit_status = 130;
			continue ;
		}
		// if (g_signal == SIGINT)
		// {
		// 	shell->exit_status = 130;
		// 	g_signal = 0;
		// 	if (!line)
		// 		continue ;
		// }
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
