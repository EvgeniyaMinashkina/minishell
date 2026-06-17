/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:24:29 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/17 10:24:30 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	set_flags_zero(t_formatspec *flags)
{
	flags->left_justify = 0;
	flags->force_sign = 0;
	flags->space = 0;
	flags->alternate = 0;
	flags->zero_pad = 0;
	flags->width = 0;
	flags->precision = -1;
	flags->specifier = '0';
}

int	ft_isspecifier(char c)
{
	if (c == 'c' || c == 's'
		|| c == 'p' || c == 'd'
		|| c == 'i' || c == 'u'
		|| c == 'x' || c == 'X' || c == '%')
		return (1);
	return (0);
}

int	write_n_chars(char c, int n)
{
	int	n_cpy;

	n_cpy = n;
	while (n-- > 0)
		ft_putchar_fd(c, 1);
	return (n_cpy);
}

char	*ft_utoa(unsigned long n)
{
	char	buf[21];
	int		i;

	i = 20;
	buf[i--] = '\0';
	if (n == 0)
		buf[i--] = '0';
	else
	{
		while (n > 0)
		{
			buf[i--] = '0' + (n % 10);
			n /= 10;
		}
	}
	return (ft_strdup(&buf[i + 1]));
}

char	*ft_get_hex(unsigned int num, const char *base)
{
	char	buffer[9];
	int		i;

	i = 8;
	buffer[i--] = '\0';
	if (num == 0)
		buffer[i--] = '0';
	else
	{
		while (num > 0)
		{
			buffer[i--] = base[num % 16];
			num /= 16;
		}
	}
	return (ft_strdup(&buffer[i + 1]));
}
