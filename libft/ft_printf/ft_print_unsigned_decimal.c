/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_decimal.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:30:43 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/18 15:30:45 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*ft_get_num_str(t_formatspec *flags, unsigned int *num)
{
	char	*num_str;

	num_str = NULL;
	if (*num == 0 && flags->precision == 0)
		num_str = ft_strdup("");
	else
		num_str = ft_utoa(*num);
	return (num_str);
}

static int	ft_get_padding_spaces(t_formatspec *flags, int *num_total_len)
{
	int		padding_spaces;

	padding_spaces = 0;
	if (flags->width > *num_total_len)
		padding_spaces = flags->width - *num_total_len;
	return (padding_spaces);
}

void	ft_print_uint(unsigned int num, t_formatspec *flags, int *total_len)
{
	char	*num_str;
	int		num_len;
	int		padding_zeros;
	int		num_total_len;

	padding_zeros = 0;
	num_str = ft_get_num_str(flags, &num);
	num_len = ft_strlen(num_str);
	if (flags->precision >= 0)
		flags->zero_pad = 0;
	if (flags->precision >= 0 && flags->precision > num_len)
		padding_zeros = flags->precision - num_len;
	else if (flags->zero_pad && !flags->left_justify && flags->width > num_len)
		padding_zeros = flags->width - num_len;
	num_total_len = padding_zeros + num_len;
	if (!flags->left_justify)
		write_n_chars(' ', ft_get_padding_spaces(flags, &num_total_len));
	write_n_chars('0', padding_zeros);
	if (num_len > 0)
		write(1, num_str, num_len);
	if (flags->left_justify)
		write_n_chars(' ', ft_get_padding_spaces(flags, &num_total_len));
	free(num_str);
	(*total_len) += num_total_len;
	(*total_len) += ft_get_padding_spaces(flags, &num_total_len);
}
