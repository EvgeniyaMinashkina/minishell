/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strljoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 15:09:46 by tkoval            #+#    #+#             */
/*   Updated: 2026/06/22 14:44:15 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strljoin(char const *dest, char const *src, size_t len)
{
	char	*res;
	size_t	res_len;
	size_t	dest_len;
	size_t	i;
	size_t	k;

	i = 0;
	k = 0;
	if (!dest)
		dest = "";
	dest_len = ft_strlen(dest);
	res_len = dest_len + len +1;
	res = (char *)malloc(sizeof(char) * res_len);
	if (!res)
		return (NULL);
	if (res_len != 1)
	{
		while (i < res_len && k < dest_len)
			res[i++] = dest[k++];
		k = 0;
		while (i < res_len && k < len)
			res[i++] = src[k++];
	}
	res[i] = '\0';
	return (res);
}
