/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 13:21:26 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/28 13:21:28 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
s1: The prefix string.
s2: The suffix string

Allocates memory (using malloc(3)) and returns a
new string, which is the result of concatenating
’s1’ and ’s2’.
*/

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	res_len;
	size_t	i;
	size_t	k;

	i = 0;
	k = 0;
	res_len = ft_strlen(s1) + ft_strlen(s2) +1;
	res = (char *)malloc(sizeof(char) * res_len);
	if (!res)
		return (NULL);
	if (res_len != 1)
	{
		while (i < res_len && k < ft_strlen(s1))
			res[i++] = s1[k++];
		k = 0;
		while (i < res_len && k < ft_strlen(s2))
			res[i++] = s2[k++];
	}
	res[i] = '\0';
	return (res);
}
