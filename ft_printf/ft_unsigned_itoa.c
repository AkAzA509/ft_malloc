/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_itoa.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggirault <ggirault@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 09:50:28 by ggirault          #+#    #+#             */
/*   Updated: 2024/11/19 14:04:26 by ggirault         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	len_calc_itoa(unsigned int n)
{
	unsigned int	len;

	len = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*alloc_itoa(unsigned int len)
{
	char	*res;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	return (res);
}

static void	convert_num(char *res, unsigned int n, unsigned int len)
{
	if (n == 0)
	{
		res[0] = '0';
		return ;
	}
	while (n > 0)
	{
		len--;
		res[len] = (n % 10) + '0';
		n /= 10;
	}
}

char	*ft_unsigned_itoa(unsigned int n)
{
	char			*res;
	unsigned int	len;

	len = len_calc_itoa(n);
	res = alloc_itoa(len);
	if (!res)
		return (NULL);
	convert_num(res, n, len);
	return (res);
}
