/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:09:32 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/28 16:09:33 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
n: The integer to convert

Allocates memory (using malloc(3)) and returns
a string representing the integer received as an
argument. Negative numbers must be handled.
*/

static int	ft_countdigits(int n)
{
	int	count;

	if (n >= 0 && n < 10)
		return (1);
	if (n < 0)
		count = 1;
	else
		count = 0;
	while (n)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*res;
	long	nbr;

	nbr = (long)n;
	len = ft_countdigits(n);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (nbr < 0)
	{
		res[0] = '-';
		nbr *= -1;
	}
	while (len > 0 && res[len - 1] != '-')
	{
		res[len - 1] = (nbr % 10) + '0';
		nbr /= 10;
		len--;
	}
	return (res);
}
