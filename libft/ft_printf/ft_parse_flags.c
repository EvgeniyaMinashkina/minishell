/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:08:31 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/17 10:08:33 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_getflagnumber(const char **format_str)
{
	int			res;
	const char	*ptr;

	ptr = *format_str;
	res = 0;
	while (*ptr && ft_isdigit(*ptr))
	{
		res = res * 10 + (*ptr - '0');
		ptr ++;
	}
	ptr--;
	*format_str = ptr;
	return (res);
}

static void	ft_parse_basic_flags(char c, t_formatspec *flags)
{
	if (c == '-')
		flags->left_justify = 1;
	else if (c == '+')
		flags->force_sign = 1;
	else if (c == '#')
		flags->alternate = 1;
	else if (c == ' ')
		flags->space = 1;
}

static const char	*ft_parse_precision(const char *ptr, t_formatspec *flags)
{
	ptr++;
	if (ft_isdigit(*ptr))
		flags->precision = ft_getflagnumber(&ptr);
	else
	{
		flags->precision = 0;
		ptr--;
	}
	return (ptr);
}

static const char	*parse_width_or_zero(const char *ptr, t_formatspec *flags)
{
	if (*ptr == '0')
		flags->zero_pad = 1;
	else
		flags->width = ft_getflagnumber(&ptr);
	return (ptr);
}

void	parse_flags(const char **format_str, t_formatspec *flags)
{
	const char	*ptr;

	ptr = *format_str;
	while (*ptr)
	{
		if (*ptr == '-' || *ptr == '+' || *ptr == '#' || *ptr == ' ')
			ft_parse_basic_flags(*ptr, flags);
		else if (*ptr == '.')
			ptr = ft_parse_precision(ptr, flags);
		else if (ft_isdigit(*ptr))
			ptr = parse_width_or_zero(ptr, flags);
		else if (ft_isspecifier(*ptr))
		{
			flags->specifier = *ptr;
			*format_str = ptr;
			return ;
		}
		else
		{
			*format_str = ptr;
			return ;
		}
		ptr++;
	}
	*format_str = ptr;
}
