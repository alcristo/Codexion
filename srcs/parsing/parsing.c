/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 14:55:38 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 12:18:20 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	ft_ispos(char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

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

int	parse_args(int argc, char **argv)
{
	int	i;

	if (argc != 9)
		return (printf("Only 8 arguments can be accepted as input\n"), 1);
	if (strcmp(argv[8], "fifo") && strcmp(argv[8], "edf"))
		return (printf("Invalid scheduler; only 'fifo' & 'edf' allowed\n"), 1);
	i = 1;
	while (i++ < argc - 2)
	{
		if (ft_ispos(argv[i]))
			return (printf("Invalid positive argument: %s\n", argv[i]), 1);
		if (ft_atol(argv[i]) == (long)INT_MAX + 1)
			return (printf("Invalid integer argument: %s\n", argv[i]), 1);
	}
	if (atoi(argv[1]) < 1 || atoi(argv[1]) > 200)
		return (printf("Coders range must be between 1 and 200\n"), 1);
	return (0);
}

t_params	*parse_params(char **argv)
{
	t_params			*params;

	params = malloc(sizeof(t_params));
	if (!params)
		return (NULL);
	if (!strcmp(argv[8], "fifo"))
		params->scheduler = FIFO;
	else if (!strcmp(argv[8], "edf"))
		params->scheduler = EDF;
	else
		return (free(params), NULL);
	params->num = atoi(argv[1]);
	params->time_burnout = atoi(argv[2]);
	params->time_compile = atoi(argv[3]);
	params->time_debug = atoi(argv[4]);
	params->time_refactor = atoi(argv[5]);
	params->required = atoi(argv[6]);
	params->cooldown = atoi(argv[7]);
	return (params);
}
