/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:36:08 by tkoval            #+#    #+#             */
/*   Updated: 2025/10/28 13:54:06 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
The strdup() function returns a pointer to a new string
which is a duplicate of the string s.
Memory for the new string is obtained with malloc(3),
and can be freed with free(3)
*/

char	*ft_strdup(const char *s)
{
	char	*res;
	int		i;
	size_t	s_len;

	i = 0;
	s_len = 0;
	while (s[s_len])
		s_len++;
	res = (char *)malloc((s_len + 1) * sizeof(char));
	if (!res)
		return (0);
	while (s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
