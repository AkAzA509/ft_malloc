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

#include "ft_printf.h"

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

int	di_format(int nb, int flag) {
	int		i;
	char	*str;
	int		tmp;

	i = 0;
	tmp = 0;
	if (flag == 2 && nb >= 0)
		tmp += ft_putchar(' ');
	if (flag == 3 && nb >= 0)
		tmp += ft_putchar('+');
	str = ft_itoa(nb);
	while (str[i] != '\0') {
		ft_putchar(str[i]);
		i++;
	}
	free(str);
	return (i + tmp);
}

int	u_format(unsigned int nb) {
	unsigned int	res;
	int				i;
	char			*str;

	res = nb;
	i = 0;
	str = ft_unsigned_itoa(res);
	while (str[i] != '\0') {
		ft_putchar(str[i]);
		i++;
	}
	free(str);
	return (i);
}
