/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Untitled-1                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 11:06:29 by marvin            #+#    #+#             */
/*   Updated: 2024/11/17 11:06:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_printf.h"

int	s_format(char *str) {
	int	i;

	i = 0;
	if (!str)
		str = "(null)";
	while (str[i] != '\0') {
		ft_putchar(str[i]);
		i++;
	}
	return (i);
}

int	x_format(unsigned long adr, char format, int flag) {
	int	i;

	i = 0;
	if (flag == 1 && adr > 0) {
		ft_putchar('0');
		if (format == 'X')
			ft_putchar('X');
		else
			ft_putchar('x');
		i += 2;
	}
	i += convert_hexa(adr, format);
	return (i);
}

int	p_format(void *adr, char format) {
	int	i;

	i = 0;
	if (adr == NULL) {
		ft_putchar('(');
		ft_putchar('n');
		ft_putchar('i');
		ft_putchar('l');
		ft_putchar(')');
		return (5);
	}
	if (format == 'p') {
		ft_putchar('0');
		ft_putchar('x');
		i += 2;
	}
	i += convert_hexa((unsigned long)adr, format);
	return (i);
}

static int	print_unsigned_uint(unsigned int nb) {
	int	len;

	len = 0;
	if (nb >= 10)
		len += print_unsigned_uint(nb / 10);
	len += ft_putchar((char)('0' + (nb % 10)));
	return (len);
}

int	di_format(int nb, int flag) {
	int			len;
	unsigned int	mag;

	len = 0;
	if (flag == 2 && nb >= 0)
		len += ft_putchar(' ');
	if (flag == 3 && nb >= 0)
		len += ft_putchar('+');
	if (nb < 0) {
		len += ft_putchar('-');
		mag = (unsigned int)(-(nb + 1)) + 1;
	}
	else
		mag = (unsigned int)nb;
	len += print_unsigned_uint(mag);
	return (len);
}

int	u_format(unsigned int nb) {
	return (print_unsigned_uint(nb));
}

static int	print_unsigned_size(size_t nb) {
	int	len;

	len = 0;
	if (nb >= 10)
		len += print_unsigned_size(nb / 10);
	len += ft_putchar((char)('0' + (nb % 10)));
	return (len);
}

int	zu_format(size_t nb) {
	return (print_unsigned_size(nb));
}

int	zdi_format(ssize_t nb, int flag) {
	int		len;
	size_t	mag;

	len = 0;
	if (nb >= 0 && flag == 2)
		len += ft_putchar(' ');
	if (nb >= 0 && flag == 3)
		len += ft_putchar('+');
	if (nb < 0) {
		len += ft_putchar('-');
		mag = (size_t)(-(nb + 1)) + 1;
	}
	else
		mag = (size_t)nb;
	len += print_unsigned_size(mag);
	return (len);
}
