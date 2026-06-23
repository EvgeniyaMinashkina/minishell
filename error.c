/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 15:38:49 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/23 18:36:37 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_error_string(char *token_value)
{
	char		*result;
	const char	*prefix;
	size_t		len;

	prefix = "minishell: syntax error near unexpected token `";
	if (!token_value)
		token_value = "newline";
	len = ft_strlen(prefix) + ft_strlen(token_value) + 2 + 1;
	result = malloc(len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, prefix, len);
	ft_strlcat(result, token_value, len);
	ft_strlcat(result, "'", len);
	return (result);
}

int	syntax_error_token(t_shell *shell, char *token_value)
{
	shell->error_msg = build_error_string(token_value);
	shell->exit_status = 2;
	return (2);
}

char	*token_type_to_str(t_token *token)
{
	if (!token)
		return ("newline");
	if (token->value)
		return (token->value);
	if (token->type == PIPE)
		return ("|");
	if (token->type == REDIR_IN)
		return ("<");
	if (token->type == REDIR_OUT)
		return (">");
	if (token->type == APPEND)
		return (">>");
	if (token->type == HEREDOC)
		return ("<<");
	return ("newline");
}

static char	*get_error_message(t_error_type type)
{
	static char	*msg[13];

	msg[0] = "syntax error: unclosed quote";
	msg[1] = "syntax error: unclosed parenthesis";
	msg[2] = "syntax error: unexpected token";
	msg[3] = "syntax error: redirection without filename";
	msg[4] = "malloc failed";
	msg[5] = "command not found";
	msg[6] = "permission denied";
	msg[7] = "is a directory";
	msg[8] = "no such file or directory";
	msg[9] = "too many arguments";
	msg[10] = "numeric argument required";
	msg[11] = "error";
	msg[12] = "syntax error: expected filename";
	return (msg[type]);
}

int	throw_error(t_shell *shell, t_error_type type)
{
	const char	*msg;
	const char	*prefix;
	size_t		len;

	msg = get_error_message(type);
	prefix = "minishell: ";
	if (!msg)
		msg = "error";
	len = ft_strlen(prefix) + ft_strlen(msg) + 1;
	shell->error_msg = malloc(len);
	if (!shell->error_msg)
		return (2);
	ft_strlcpy(shell->error_msg, prefix, len);
	ft_strlcat(shell->error_msg, msg, len);
	shell->exit_status = 2;
	return (2);
}
