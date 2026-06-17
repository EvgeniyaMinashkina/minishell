/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:34:24 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/17 10:34:26 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_puthex(unsigned long n, int *total_len)
{
	const char	*base;

	base = "0123456789abcdef";
	if (n >= 16)
		ft_puthex(n / 16, total_len);
	ft_putchar_fd(base[n % 16], 1);
	(*total_len)++;
}

int	ft_count_hex_digits(unsigned long num)
{
	int	count;

	count = 1;
	while (num >= 16)
	{
		num /= 16;
		count++;
	}
	return (count);
}

static void	ft_print_null_pointer(t_formatspec *flags, int *total_len)
{
	int	i;

	i = 0;
	if (flags->left_justify == 1)
		write(1, "(nil)", 5);
	while (i < (flags->width - 5))
	{
		ft_putchar_fd(' ', 1);
		(*total_len)++;
		i++;
	}
	if (flags->left_justify == 0)
		write(1, "(nil)", 5);
	(*total_len) += 5;
}

static void	ft_print_hex_num(unsigned long *hex_ptr, int *total_len)
{
	write(1, "0x", 2);
	ft_puthex(*hex_ptr, total_len);
}

void	ft_print_pointer(void *ptr, t_formatspec *flags, int *total_len)
{
	unsigned long	hex_ptr;
	int				ptr_len;
	char			print_sign;

	hex_ptr = (unsigned long)ptr;
	if (!ptr)
	{
		ft_print_null_pointer(flags, total_len);
		return ;
	}
	ptr_len = ft_count_hex_digits(hex_ptr) + 2;
	if (flags->zero_pad == 1 && flags->left_justify == 0)
		print_sign = '0';
	else
		print_sign = ' ';
	if (flags->left_justify == 1)
		ft_print_hex_num(&hex_ptr, total_len);
	write_n_chars(print_sign, flags->width - ptr_len);
	if ((flags->width - ptr_len) >= 0)
		(*total_len) += (flags->width - ptr_len);
	if (flags->left_justify == 0)
		ft_print_hex_num(&hex_ptr, total_len);
	(*total_len) += 2;
}
