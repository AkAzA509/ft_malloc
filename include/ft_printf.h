/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggirault <ggirault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 15:22:34 by ggirault          #+#    #+#             */
/*   Updated: 2024/11/20 15:22:34 by ggirault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

int		ft_printf(const char *format, ...);
int		ft_putchar(char c);
int		format_checker(const char *str, va_list ap, int i, int *total_len);
int		flag_checker(const char *str, int i);
int		s_format(char *str);
int		x_format(unsigned long adr, char format, int flag);
int		p_format(void *adr, char format);
int		convert_hexa(unsigned long nb, char format);
int		di_format(int nb, int flag);
int		u_format(unsigned int nb);
int		zu_format(size_t nb);
int		zdi_format(ssize_t nb, int flag);
void	len_calc(int *total_len, int len);

#endif
