/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 14:55:38 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 15:28:29 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"
int	ft_ispos(char *s) {
	size_t	i;

	i = 0;
	while (s[i++])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
	}
	return (0);
}

int	parse_args(int argc, char **argv) {
	size_t	i;

	if (argc != 9)
		return (print("Only 8 arguments can be accepted as input\n"), 1);
	if (strcmp(argv[8], "fifo") && strcmp(argv[8], "edf"))
		return (print("Invalid scheduler; only 'fifo' & 'edf' allowed\n"), 1);
	i = 1;
	while (i++ < argc - 1)
	{
		if (ft_ispos(argv[i])
			return (printf("Invalid argument: %s\n"), 1);
	}
	if (atoi(argv[1] < 2))
		return (print("At least two coders are required to compile\n"), 1);
	return (0);
}

t_param	*parse_params(char **argv) {
	t_params			*params;
	//enum e_scheduler	sch;

	params = malloc(sizeof(t_params));
	if (!params)
		return NULL;
	if (strcmp(argv[8], "fifo"))
		params->scheduler = FIFO;
	else if (strcmp(argv[8], "edf"))
		params->scheduler = EDF;
	params->num = atoi(argv[1]);
	params->time_burnout = atoi(argv[2]);
	params->time_compile = atoi(argv[3]);
	params->time_debug = atoi(argv[4]);
	params->time_refactor = atoi(argv[5]);
	params->required = atoi(argv[6]);
	params->cooldown = atoi(argv[7]);
	return params;
}
