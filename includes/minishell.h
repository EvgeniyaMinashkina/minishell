/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:13:01 by yminashk          #+#    #+#             */
/*   Updated: 2026/06/23 00:29:44 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
# include "libft.h"

extern volatile sig_atomic_t	g_signal;

typedef enum e_quote_state
{
	NONE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_state;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	AND,
	OR,
	LPAREN,
	RPAREN
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	t_quote_state	quote_state;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	t_token	*tokens;
	t_cmd	*cmd_list;
	char	**envp;
	int		exit_status;
	char	*error_msg;
}	t_shell;

typedef enum e_error_type
{
	ERR_UNCLOSED_QUOTE,
	ERR_UNCLOSED_PAREN,
	ERR_UNEXPECTED_TOKEN,
	ERR_REDIR_NO_FILE,
	ERR_MALLOC,
	ERR_CMD_NOT_FOUND,
	ERR_PERMISSION_DENIED,
	ERR_IS_DIRECTORY,
	ERR_NO_SUCH_FILE,
	ERR_TOO_MANY_ARGS,
	ERR_NUMERIC_ARG_REQUIRED,
	ERR_GENERAL,
	ERR_EXPECTED_FILENAME
}	t_error_type;

/*LEXER dir. */
/* lexer.c */
t_token	*lexer(t_shell *shell, char *str);
/*lexer/helper.c */
bool	is_space(char c);
int		is_operator(char *str);
void	free_tokens(t_token *tokens);
void	toggle_quote_state(t_quote_state *state, char c);

/* syntax/syntax_check.c */
int		syntax_check(t_shell *shell);
int		is_binary_operator(t_token *token);
int		is_redirection(t_token *token);
/* syntax/parenthesis_check.c */
int		check_parenthesis(t_shell *shell);

/* parser/parser.c */
t_cmd	*parse_tokens(t_shell *shell);
void	print_cmds(t_cmd *cmd_list);

/* parser/parser_helpers.c */
t_cmd	*new_cmd(void);
void	add_redir(t_cmd *cmd, t_token_type type, char *filename);
void	add_argv(t_cmd *cmd, char *value);
void	add_cmd_back(t_cmd **cmd_list, t_cmd *new_cmd);

/* error.c */
int		throw_error(t_shell *shell, t_error_type type);
char	*token_type_to_str(t_token *token);
int		syntax_error_token(t_shell *shell, char *token_value);

/* EXPANDER */
/* expander/expander.c */
void	expand_cmds(t_shell *shell);

/* expander/expand_string.c */
void	append_str(char **res, char *str);
char	*expand_string(char *str, t_shell *shell);
char	*get_variable_name(char *str);

/* expander/env_search.c */
char	*get_variable_name(char *str);
char	*get_env_value(char **envp, const char *name);

/* expander/ft_strljoin.c */
char	*ft_strljoin(char const *dest, char const *src, size_t len);

/* expander/expand_utils.c */
int		is_valid_char(char c);
void	append_str(char **res, char *str);
void	append_str_norm(char **res, char *str, int *i);
void	append_var(char *str, int *i, t_shell *shell, char **res);
void	toggle_or_add_quote(t_quote_state *quote, char c, char **res);

/* ************************************************************************** */
/*                                 EXECUTOR                                   */
/* ************************************************************************** */

int		execute_single_command(t_cmd *cmd, t_shell *shell);
void	wait_last_pid(pid_t last_pid, t_shell *shell);
int		execute_pipeline(t_cmd *cmd_list, t_shell *shell);
int		execute_command(t_cmd *cmd, int in_fd, int out_fd, t_shell *shell);
int		setup_pipe(t_cmd *cmd, int pipefd[2]);
int 	handle_exec_error(int prev_fd, t_cmd *cmd, int pipefd[2]);
void	update_fds(int *prev_fd, t_cmd *cmd, int pipefd[2]);
int		save_stdio(int *stdin_copy, int *stdout_copy);
void	restore_stdio(int stdin_copy, int stdout_copy);

/* ************************************************************************** */
/*                                 BUILTINS                                   */
/* ************************************************************************** */

int		is_valid_identifier(char *str);
int		is_builtin(char *cmd);
int		is_parent_builtin(char *cmd);
int		builtin_pwd(void);
int		builtin_echo(char **argv);
int		builtin_env(char **envp);
void	export_error(char *arg);
int		is_valid_exit_number(char *str);
int		exec_builtin(t_cmd *cmd, t_shell *shell);

/* ************************************************************************** */
/*                                  SYSTEM                                    */
/* ************************************************************************** */

int		apply_redirections(t_redir *redirs, t_shell *shell);
int		heredoc_pipe(char *delim, t_shell *shell);

/* ************************************************************************** */
/*                                   PATH                                     */
/* ************************************************************************** */

char	*find_cmd_path(char *cmd, char **envp);

/* ************************************************************************** */
/*                                  SIGNALS                                   */
/* ************************************************************************** */

void	init_signals_prompt(void);
void	init_signals_child(void);

/* ************************************************************************** */
/*                                    ENV                                     */
/* ************************************************************************** */

int		env_count(char **envp);
char	*env_get(char **envp, const char *key);
int		env_set(char ***envp, const char *key, const char *value);
int		env_unset(char ***envp, const char *key);
char	**env_init(char **envp);
char	*create_env_line(const char *key, const char *value);
void	free_env(char **envp);

/* ************************************************************************** */
/*                                   FREE                                     */
/* ************************************************************************** */
void	free_cmds(t_cmd *cmds);
void	free_redirs(t_redir *redirs);
void	shell_exit(t_shell *shell, int status);

#endif