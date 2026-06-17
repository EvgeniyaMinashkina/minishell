/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_decimal.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:29:44 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/18 15:29:45 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*ft_get_prefix(int num, t_formatspec *flags)
{
	char	*prefix;

	prefix = NULL;
	if (num < 0)
		prefix = ft_strdup("-");
	else if (flags->force_sign)
		prefix = ft_strdup("+");
	else if (flags->space && num < 0)
		prefix = ft_strdup("-");
	else if (flags->space && num >= 0 && flags->force_sign == 0)
		prefix = ft_strdup(" ");
	return (prefix);
}

static char	*ft_get_num_str(int num, int *num_len, t_formatspec *flags)
{
	char	*num_str;

	if (num == 0 && flags->precision == 0)
		num_str = ft_strdup("");
	else
	{
		if (num < 0)
			num_str = ft_utoa((long)num * -1);
		else
			num_str = ft_utoa((long)num);
		*num_len = (int)ft_strlen(num_str);
	}
	return (num_str);
}

static int	ft_get_zero_padding(t_formatspec *flags, int *prefix_len,
	int *num_len)
{
	int		padding_zeros;

	padding_zeros = 0;
	if (flags->precision >= 0 && flags->precision > *num_len)
		padding_zeros = flags->precision - *num_len;
	else if (flags->zero_pad && flags->precision == -1 && !flags->left_justify
		&& flags->width > (*num_len + *prefix_len))
		padding_zeros = flags->width - *num_len - *prefix_len;
	return (padding_zeros);
}

static void	ft_put_dec(t_formatspec *flags, int *num_total_len, char *prefix,
	char *num_str)
{
	int		prefix_len;
	int		num_len;

	prefix_len = 0;
	num_len = ft_strlen(num_str);
	if (prefix)
		prefix_len = 1;
	if (!flags->left_justify)
		write_n_chars(' ', flags->width - *num_total_len);
	write(1, prefix, prefix_len);
	write_n_chars('0', ft_get_zero_padding(flags, &prefix_len, &num_len));
	if (num_len > 0)
		write(1, num_str, num_len);
	if (flags->left_justify)
		write_n_chars(' ', flags->width - *num_total_len);
}

void	ft_print_decimal(int num, t_formatspec *flags, int *total_len)
{
	char	*num_str;
	int		num_len;
	char	*prefix;
	int		prefix_len;
	int		num_total_len;

	prefix_len = 0;
	num_len = 0;
	num_str = ft_get_num_str(num, &num_len, flags);
	prefix = ft_get_prefix(num, flags);
	if (prefix)
		prefix_len = 1;
	if (flags->precision >= 0)
		flags->zero_pad = 0;
	num_total_len = prefix_len + num_len;
	num_total_len += ft_get_zero_padding(flags, &prefix_len, &num_len);
	ft_put_dec(flags, &num_total_len, prefix, num_str);
	(*total_len) += num_total_len;
	if (flags->width > num_total_len)
		(*total_len) += flags->width - num_total_len;
	free(num_str);
	free(prefix);
}
