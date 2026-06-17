/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 11:11:52 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/17 11:11:53 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	ft_print_arg(va_list args, t_formatspec *flags, int *total_len)
{
	if (flags->specifier == 'c')
		ft_print_c((char)va_arg(args, int), flags, total_len);
	else if (flags->specifier == 's')
		ft_print_s(va_arg(args, char *), flags, total_len);
	else if (flags->specifier == 'p')
		ft_print_pointer(va_arg(args, void *), flags, total_len);
	else if (flags->specifier == 'd' || flags->specifier == 'i')
		ft_print_decimal(va_arg(args, int), flags, total_len);
	else if (flags->specifier == 'u')
		ft_print_uint(va_arg(args, unsigned int), flags, total_len);
	else if (flags->specifier == 'x' || flags->specifier == 'X')
		ft_print_x(va_arg(args, unsigned int), flags, total_len);
	else if (flags->specifier == '%')
	{
		write(1, "%", 1);
		(*total_len)++;
	}
}

static int	ft_process_format_string(const char *ptr, va_list args,
									t_formatspec *flags)
{
	int	total_len;

	total_len = 0;
	while (*ptr)
	{
		if (*ptr == '%')
		{
			ptr++;
			parse_flags(&ptr, flags);
			ft_print_arg(args, flags, &total_len);
			set_flags_zero(flags);
		}
		else
			total_len += write_n_chars(*ptr, 1);
		ptr++;
	}
	return (total_len);
}

int	ft_printf(const char *str, ...)
{
	va_list			args;
	t_formatspec	flags;
	int				total_len;

	if (!str || *str == '\0')
		return (0);
	set_flags_zero(&flags);
	va_start(args, str);
	total_len = ft_process_format_string(str, args, &flags);
	va_end(args);
	return (total_len);
}
