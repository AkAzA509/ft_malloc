/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:50:29 by marvin            #+#    #+#             */
/*   Updated: 2024/11/17 17:50:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	len_calc(int *total_len, int len) {
	*total_len += len;
}

int	flag_checker(const char *str, int i) {
	int	flag;

	flag = 0;
	if (str[i + 1] == '#') {
		flag = 1;
		i++;
	}
	if (str[i + 1] == ' ' ) {
		flag = 2;
		i += 2;
	}
	if (str[i + 1] == '+') {
		flag = 3;
		i++;
	}
	return (flag);
}

int	format_checker(const char *str, va_list ap, int i, int *total_len) {
	int	flag;

	flag = flag_checker(str, i);
	if (flag > 0)
		i++;
	if (str[i + 1] == 'c')
		len_calc(total_len, ft_putchar(va_arg(ap, int)));
	if (str[i + 1] == 's')
		len_calc(total_len, s_format(va_arg(ap, char *)));
	if (str[i + 1] == 'p')
		len_calc(total_len, p_format(va_arg(ap, void *), 'p'));
	if (str[i + 1] == 'd' || str[i + 1] == 'i')
		len_calc(total_len, di_format(va_arg(ap, int), flag));
	if (str[i + 1] == 'u')
		len_calc(total_len, u_format(va_arg(ap, unsigned int)));
	if (str[i + 1] == 'X' || str[i + 1] == 'x')
		len_calc(total_len, x_format(va_arg(ap, unsigned int), str[i + 1], flag));
	if (str[i + 1] == '%')
		len_calc(total_len, ft_putchar('%'));
	i++;
	return (i);
}

int	ft_printf(const char *str, ...) {
	va_list	ap;
	int		i;
	int		total_len;

	total_len = 0;
	i = 0;
	va_start(ap, str);
	while (str[i] != '\0')
	{
		if (str[i] == '%')
			i = format_checker(str, ap, i, &total_len);
		else {
			ft_putchar(str[i]);
			len_calc(&total_len, 1);
		}
		i++;
	}
	va_end(ap);
	return (total_len);
}

/*int main(void)
{
	int	nb;

	nb = ft_printf("Hello world %c\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %s\n", "lkdsej");
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %p\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %d\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %i\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %u\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %x\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %X\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %#x\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %#X\n", );
	ft_printf("%d", nb);
	nb = ft_printf("Hello world % d\n", 54);
	ft_printf("%d", nb);
	nb = ft_printf("Hello world % i\n", 54);
	ft_printf("%d", nb);
	nb = ft_printf("Hello world %#p\n", );
	ft_printf("%d", nb);

	return (0);
}*/
