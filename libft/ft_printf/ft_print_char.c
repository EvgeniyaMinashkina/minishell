/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:26:33 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/18 15:26:34 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_print_c(char c, t_formatspec *flags, int *total_len)
{
	if (flags->left_justify == 1)
		write(1, &c, 1);
	if (flags->width > 1)
		(*total_len) += write_n_chars(' ', flags->width - 1);
	if (flags->left_justify == 0)
		write(1, &c, 1);
	(*total_len)++;
}
