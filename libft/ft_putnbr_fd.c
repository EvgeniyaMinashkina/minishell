/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:16:46 by tkoval            #+#    #+#             */
/*   Updated: 2025/05/28 18:16:47 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
n: The integer to output.
fd: The file descriptor on which to write.

Outputs the integer ’n’ to the specified file
descriptor.
*/

void	ft_putnbr_fd(int n, int fd)
{
	char	c;
	long	nb;

	nb = (long)n;
	if (n < 0)
	{
		write(fd, "-", 1);
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr_fd(nb / 10, fd);
	}
	c = nb % 10 + '0';
	write (fd, &c, 1);
}
