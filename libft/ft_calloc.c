/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:23:43 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/26 18:23:44 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;
	size_t	total;

	if (nmemb == 0 || size == 0)
	{
		res = malloc(0);
		if (!res)
			return (NULL);
		return (res);
	}
	total = nmemb * size;
	if (nmemb != 0 && total / nmemb != size)
		return (NULL);
	res = (void *)malloc(total);
	if (!res)
		return (NULL);
	ft_bzero(res, total);
	return (res);
}
