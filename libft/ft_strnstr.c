/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:26:01 by tkoval            #+#    #+#             */
/*   Updated: 2025/10/28 13:57:43 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
The strnstr() function locates the first occurrence
of the null-terminated string little in the string big,
where not more than len characters are searched.
Characters that appear after a ‘\0’ character are not searched.
*/

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;
	size_t	i;
	size_t	k;

	little_len = 0;
	i = 0;
	while (little[little_len])
		little_len++;
	if (little_len == 0)
		return ((char *)big);
	while (i < len && big[i])
	{
		k = 0;
		while (little[k] && (i + k) < len && big[i + k] == little[k])
			k++;
		if (little[k] == '\0')
			return ((char *)&big[i]);
		i++;
	}
	return (0);
}
