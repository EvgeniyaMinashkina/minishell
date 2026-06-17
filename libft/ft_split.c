/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 15:41:26 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/28 15:41:28 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
s: The string to be split.
c: The delimiter character.

Allocates memory (using malloc(3)) and returns an
array of strings obtained by splitting ’s’ using
the character ’c’ as a delimiter. The array must
end with a NULL pointer.
*/

static int	ft_countelements(char const *str, char separator)
{
	int	i;
	int	count;
	int	word_started;

	i = 0;
	count = 0;
	word_started = 0;
	while (str[i])
	{
		if (str[i] != separator && !word_started)
		{
			word_started = 1;
			count++;
		}
		else if (str[i] == separator)
			word_started = 0;
		i++;
	}
	return (count);
}

static char	*ft_getword(char const *str, char charset)
{
	int		len;
	char	*word;
	int		i;

	len = 0;
	while (str[len] && str[len] != charset)
		len++;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (0);
	i = 0;
	while (str[i] && str[i] != charset)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_free_split(char **split, size_t count)
{
	if (!split)
		return ;
	while (count > 0)
	{
		count--;
		free(split[count]);
	}
	free(split);
}

static int	fill_words(char **result, const char *s, char c)
{
	size_t	i;
	size_t	word;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (!s[i])
			break ;
		result[word] = ft_getword(&s[i], c);
		if (!result[word])
		{
			ft_free_split(result, word);
			return (0);
		}
		while (s[i] && s[i] != c)
			i++;
		word++;
	}
	result[word] = NULL;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_countelements(s, c) + 1));
	if (!result)
		return (NULL);
	if (!fill_words(result, s, c))
		return (NULL);
	return (result);
}
