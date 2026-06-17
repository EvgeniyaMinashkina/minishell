/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:38:49 by tkoval            #+#    #+#             */
/*   Updated: 2026/04/15 15:34:10 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	syntax_error_token(t_shell *shell, char *token_value)
// {
// 	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
// 	ft_putstr_fd(token_value, 2);
// 	ft_putendl_fd("'", 2);
// 	shell->exit_status = 2;
// 	return (2);
// }

char	*build_error_string(char *token_value)
{
	char	*result;
	const char	*prefix = "minishell: syntax error near unexpected token `";
	size_t	len;

	if (!token_value)
		token_value = "newline";
	len = ft_strlen(prefix) + ft_strlen(token_value) + 2 + 1;
	result = malloc(len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, prefix, len);
	ft_strlcat(result, token_value, len);
	ft_strlcat(result, "`", len);
	return (result);
}

int syntax_error_token(t_shell *shell, char *token_value)
{
	shell->syntax_error_msg = build_error_string(token_value);
	shell->exit_status = 2;
	return (2);
}

char *token_type_to_str(t_token *token)
{
	if (!token)
		return ("newline");
	if (token->value)
		return (token->value);
	if (token->type == PIPE)
		return ("|");
	if (token->type == AND)
		return ("&&");
	if (token->type == OR)
		return ("||");
	if (token->type == REDIR_IN)
		return ("<");
	if (token->type == REDIR_OUT)
		return (">");
	if (token->type == APPEND)
		return (">>");
	if (token->type == HEREDOC)
		return ("<<");
	if (token->type == LPAREN)
		return ("(");
	if (token->type == RPAREN)
		return (")");
	return ("newline");
}

static char	*get_error_message(t_error_type type)
{
	static char *messages[] = {
		"syntax error: unclosed quote",
		"syntax error: unclosed parenthesis",
		"syntax error: unexpected token",
		"syntax error: redirection without filename",
		"malloc failed",
		"command not found",
		"permission denied",
		"is a directory",
		"no such file or directory",
		"too many arguments",
		"numeric argument required",
		"error",
		"syntax error: expected filename"
	};

	return (messages[type]);
}

// int	throw_error(t_shell *shell, t_error_type type)
// {
// 	ft_putstr_fd("minishell: ", 2);
// 	ft_putendl_fd(get_error_message(type), 2);
// 	shell->exit_status = 2;
// 	return (2);
// }

int throw_error(t_shell *shell, t_error_type type)
{
	const char *msg = get_error_message(type);
	const char *prefix = "minishell: ";
	size_t len;

	if (!msg)
		msg = "error";

	// Allocate enough space for prefix + msg + null terminator
	len = ft_strlen(prefix) + ft_strlen(msg) + 1;
	shell->syntax_error_msg = malloc(len);
	if (!shell->syntax_error_msg)
		return (2);

	// Copy prefix and concatenate message
	ft_strlcpy(shell->syntax_error_msg, prefix, len);
	ft_strlcat(shell->syntax_error_msg, msg, len);

	shell->exit_status = 2;

	// Optional: still print to stderr
	// ft_putendl_fd(shell->syntax_error_msg, 2);

	return (2);
}
