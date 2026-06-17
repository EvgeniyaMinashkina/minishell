/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:43:55 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/26 18:43:56 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
s: The original string from which to create the
substring.
start: The starting index of the substring within
’s’.
len: The maximum length of the substring.

Allocates memory (using malloc(3)) and returns a
substring from the string ’s’.
The substring starts at index ’start’ and has a
maximum length of ’len.
*/

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	s_len;
	size_t	i;
	size_t	substr_len;

	i = 0;
	s_len = 0;
	while (s[s_len])
		s_len++;
	if (start >= s_len)
		substr_len = 0;
	else if (len > s_len - start)
		substr_len = s_len - start;
	else
		substr_len = len;
	res = (char *)malloc(sizeof(char) * (substr_len + 1));
	if (!res)
		return (NULL);
	while (i < substr_len)
	{
		res[i] = s[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
