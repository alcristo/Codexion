/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 11:26:23 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/25 14:58:24 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	fullnum(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

static long	ft_atol(const char *num)
{
	long	n;
	int		neg;

	n = 0;
	neg = 1;
	while ((*num >= 9 && *num <= 13) || *num == ' ')
		num++;
	if (*num == '+' || *num == '-')
	{
		if (*num == '-')
			neg *= -1;
		num++;
	}
	while (*num >= '0' && *num <= '9')
	{
		n *= 10;
		n += *num - '0';
		if (n > (long)INT_MAX)
			return ((long)INT_MAX + 1);
		num++;
	}
	return (n * neg);
}

int	parse_args(int argc, char **argv)
{
	int	i;

	if (strcmp(argv[8], "fifo") && strcmp(argv[8], "edf"))
		return (printf("Invalid scheduler\n"), 1);
	i = 0;
	while (++i < argc - 1)
	{
		if (fullnum(argv[i]))
			return (printf("Invalid positive argument: %s\n", argv[i]), 1);
		if (ft_atol(argv[i]) == (long)INT_MAX + 1)
			return (printf("Invalid integer argument: %s\n", argv[i]), 1);
	}
	if (atoi(argv[1]) < 1 || atoi(argv[1]) > 200)
		return (printf("Invalid number of coders (range: 1-200)\n"), 1);
	return (0);
}
