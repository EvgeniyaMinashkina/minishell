/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoval <tkoval@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:15:17 by tkoval            #+#    #+#             */
/*   Updated: 2025/06/06 16:15:18 by tkoval           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "../libft.h"

typedef struct s_formatspec
{
	int		left_justify;	/*-*/
	int		force_sign;		/*+*/
	int		space;			/* */
	int		alternate;		/*#*/
	int		zero_pad;		/*0*/
	int		width;			/*Minimum field width (default 0)*/
	int		precision;		/* Precision (default -1 if not specified)*/
	char	specifier;		/*c, s, p, d, i, u, x, X, %*/
}	t_formatspec;

int		ft_printf(const char *str, ...);

//ft_parse_flags
void	parse_flags(const char **format_str, t_formatspec *flags);

//ft_helper
void	set_flags_zero(t_formatspec *flags);
int		ft_isspecifier(char c);
int		write_n_chars(char c, int n);
char	*ft_utoa(unsigned long n);
char	*ft_get_hex(unsigned int num, const char *base);

//ft_print_char
void	ft_print_c(char c, t_formatspec *flags, int *total_len);

//ft_print_string
void	ft_print_s(char *str, t_formatspec *flags, int *total_len);

//ft_print_pointer
void	ft_puthex(unsigned long n, int *total_len);
int		ft_count_hex_digits(unsigned long num);
void	ft_print_pointer(void *ptr, t_formatspec *flags, int *total_len);

//ft_print_decimal
void	ft_print_decimal(int num, t_formatspec *flags, int *total_len);

//ft_print_unsigned_decimal
void	ft_print_uint(unsigned int num, t_formatspec *flags,
			int *total_len);

//ft_print_hex
void	ft_print_x(unsigned int num, t_formatspec *flags, int *total_len);

#endif
