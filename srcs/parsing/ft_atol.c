/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 10:06:14 by alcristo          #+#    #+#             */
/*   Updated: 2026/07/09 14:17:14 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/push_swap.h"

long	ft_atol(const char *arg)
{
	long	n;
	int		neg;

	n = 0;
	neg = -1;
	while ((*arg >= 9 && *arg <= 13) || *arg == ' ')
		arg++;
	if (*arg == '+' || *arg == '-')
	{
		if (*arg == '-')
			neg *= -1;
		arg++;
	}
	while (ft_isdigit(*arg))
	{
		n *= 10;
		n += *arg - '0';
		if (n > 2147483647)
			return (2147483648);
		arg++;
	}
	return (n * neg);
}
