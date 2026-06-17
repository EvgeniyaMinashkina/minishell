/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_string.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:32:19 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/17 10:32:21 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_s(char *str, t_formatspec *flags, int *total_len)
{
	int		str_len;
	int		i;
	char	*str_dup;

	i = 0;
	str_dup = str;
	if (str == NULL && flags->precision >= 0 && flags->precision < 6)
	{
		(*total_len) += write_n_chars(' ', flags->width);
		return ;
	}
	else if (str == NULL)
		str_dup = "(null)";
	if (flags->precision != -1
		&& (size_t)flags->precision <= ft_strlen(str_dup))
		str_len = flags->precision;
	else
		str_len = ft_strlen(str_dup);
	while (i < str_len && flags->left_justify == 1)
		ft_putchar_fd(str_dup[i++], 1);
	if (str_len < flags->width)
		(*total_len) += write_n_chars(' ', flags->width - str_len);
	while (i < str_len && flags->left_justify == 0)
		ft_putchar_fd(str_dup[i++], 1);
	(*total_len) += str_len;
}
