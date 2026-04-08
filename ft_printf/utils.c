/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggirault <ggirault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:16:13 by ggirault          #+#    #+#             */
/*   Updated: 2024/11/19 14:16:13 by ggirault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	convert_hexa(unsigned long nb, char format) {
	int	i;

	i = 0;
	if (nb >= 16) {
		i += convert_hexa(nb / 16, format);
		i += convert_hexa(nb % 16, format);
	}
	else {
		if (format == 'X')
			i += ft_putchar("0123456789ABCDEF"[nb % 16]);
		else
			i += ft_putchar("0123456789abcdef"[nb % 16]);
	}
	return (i);
}
