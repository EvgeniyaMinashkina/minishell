/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:38:55 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/26 15:38:56 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*src;
	size_t				i;

	src = (const unsigned char *)s;
	i = 0;
	while (n > 0)
	{
		if (src[i] == (unsigned char)c)
			return ((void *)(src + i));
		n--;
		i++;
	}
	return (NULL);
}
