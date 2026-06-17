/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:31:28 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/18 15:31:29 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*ft_get_prefix(t_formatspec *flags, unsigned int *num,
	int *prefix_len)
{
	char	*prefix;

	prefix = NULL;
	if (flags->alternate == 1 && *num != 0)
	{
		*prefix_len = 2;
		if (flags->specifier == 'x')
			prefix = "0x";
		else if (flags->specifier == 'X')
			prefix = "0X";
	}
	return (prefix);
}

static char	*ft_get_hex_number(t_formatspec *flags, unsigned int *num,
	int *hex_len)
{
	char	*hex_num;

	hex_num = NULL;
	if (*num == 0 && flags->precision == 0)
		hex_num = ft_strdup("");
	else if (flags->specifier == 'x')
		hex_num = ft_get_hex(*num, "0123456789abcdef");
	else if (flags->specifier == 'X')
		hex_num = ft_get_hex(*num, "0123456789ABCDEF");
	*hex_len = ft_strlen(hex_num);
	return (hex_num);
}

static int	get_padding_zeros(t_formatspec *flags, int *hex_len,
	int *prefix_len)
{
	int	padding_zeros;

	padding_zeros = 0;
	if (flags->precision >= 0 && flags->precision > *hex_len)
		padding_zeros = flags->precision - *hex_len;
	else if (flags->zero_pad && flags->width > (*hex_len + *prefix_len))
		padding_zeros = flags->width - (*hex_len + *prefix_len);
	return (padding_zeros);
}

static int	get_padding_spaces(t_formatspec *flags, int *num_total_len)
{
	int	padding_spaces;

	padding_spaces = 0;
	if (flags->width > *num_total_len)
		padding_spaces = flags->width - *num_total_len;
	return (padding_spaces);
}

void	ft_print_x(unsigned int num, t_formatspec *flags, int *total_len)
{
	int		prefix_len;
	char	*prefix;
	char	*hex_num;
	int		num_total_len;
	int		hex_len;

	prefix_len = 0;
	hex_len = 0;
	if (flags->precision != -1 || flags->left_justify == 1)
		flags->zero_pad = 0;
	prefix = ft_get_prefix(flags, &num, &prefix_len);
	hex_num = ft_get_hex_number(flags, &num, &hex_len);
	num_total_len = prefix_len + hex_len;
	num_total_len += get_padding_zeros(flags, &hex_len, &prefix_len);
	if (!flags->left_justify)
		write_n_chars(' ', get_padding_spaces(flags, &num_total_len));
	if (prefix_len)
		write(1, prefix, prefix_len);
	write_n_chars('0', get_padding_zeros(flags, &hex_len, &prefix_len));
	if (hex_len > 0)
		write(1, hex_num, hex_len);
	if (flags->left_justify)
		write_n_chars(' ', get_padding_spaces(flags, &num_total_len));
	free(hex_num);
	(*total_len) += num_total_len + get_padding_spaces(flags, &num_total_len);
}
