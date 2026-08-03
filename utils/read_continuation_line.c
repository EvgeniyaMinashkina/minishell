/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_continuation_line.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yminashk <yminashk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 12:37:19 by yminashk          #+#    #+#             */
/*   Updated: 2026/08/02 12:47:24 by yminashk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_chars(char *line)
{
	char	buf[2];
	char	*tmp;
	int		n;

	while (1)
	{
		n = read(STDIN_FILENO, buf, 1);
		if (g_signal == SIGINT)
			return (free(line), NULL);
		if (n <= 0 || buf[0] == '\n')
			break ;
		buf[1] = '\0';
		tmp = ft_strjoin(line, buf);
		free(line);
		line = tmp;
		if (!line)
			return (NULL);
	}
	if (n == 0 && !line[0])
		return (free(line), NULL);
	return (line);
}

char	*read_continuation_line(void)
{
	char	*line;

	line = ft_strdup("");
	if (!line)
		return (NULL);
	write(STDOUT_FILENO, "> ", 2);
	return (read_chars(line));
}

static int	has_unclosed_quote(char *str)
{
	t_quote_state	state;
	int				i;

	state = NONE;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' && state == NONE)
			state = SINGLE_QUOTE;
		else if (str[i] == '\'' && state == SINGLE_QUOTE)
			state = NONE;
		else if (str[i] == '"' && state == NONE)
			state = DOUBLE_QUOTE;
		else if (str[i] == '"' && state == DOUBLE_QUOTE)
			state = NONE;
		i++;
	}
	return (state != NONE);
}

char	*read_complete_input(char *line)
{
	char	*next;
	char	*tmp;

	while (has_unclosed_quote(line))
	{
		next = read_continuation_line();
		if (!next)
		{
			free(line);
			return (ft_strdup(""));
		}
		tmp = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(tmp, next);
		free(tmp);
		free(next);
	}
	return (line);
}
