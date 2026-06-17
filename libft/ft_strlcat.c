/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:41:22 by tkoval            #+#    #+#             */
/*   Updated: 2025/10/28 13:55:06 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 The strlcat() function appends the NUL-terminated string src to the end of dst.
 It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.
*/

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;

	i = 0;
	dst_len = 0;
	src_len = 0;
	while (src[src_len])
		src_len++;
	while (dst[dst_len] && dst_len < size)
		dst_len++;
	if (dst_len == size)
		return (size + src_len);
	i = 0;
	while ((dst_len + i + 1) < size && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	if (dst_len + i < size)
		dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
