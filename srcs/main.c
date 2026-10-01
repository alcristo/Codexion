/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 11:05:27 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:34:07 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	free_sim(t_sim *sim)
{
	if (sim->dongles)
		free_dongles(sim->dongles);
	if (sim->heap)
		free_heap(sim->heap);
	pthread_mutex_destroy(&sim->mutex);
	pthread_cond_destroy(&sim->cond);
	free(sim);
}

static void	init_params(t_sim *sim, char **argv)
{
	sim->number = atoi(argv[1]);
	sim->time_burnout = atoi(argv[2]) * 1000L;
	sim->time_compile = atoi(argv[3]) * 1000L;
	sim->time_debug = atoi(argv[4]) * 1000L;
	sim->time_refactor = atoi(argv[5]) * 1000L;
	sim->required = atoi(argv[6]);
	sim->cooldown = atoi(argv[7]) * 1000L;
	if (!strcmp(argv[8], "edf"))
		sim->scheduler = 1;
}

static int	init_sim(t_sim *sim)
{
	int	i;

	if (pthread_mutex_init(&sim->mutex, NULL))
		return (0);
	if (pthread_cond_init(&sim->cond, NULL))
		return (0);
	sim->dongles = malloc(sim->number * sizeof(pthread_mutex_t));
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->number)
	{
		if (pthread_mutex_init(&sim->dongles[i], NULL))
			return (0);
		i++;
	}
	sim->heap = create_heap(sim);
	if (!sim->heap)
		return (0);
	if (create_coders)
	return (1);
}

int	main(int argc, char **argv)
{
	t_sim	*sim;

	if (argc != 9)
		return (printf("Only 8 arguments are accepted"), 0);
	if (parse_args(argc, argv))
		return (0);
	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (0);
	memset(sim, 0, sizeof(t_sim));
	init_params(sim, argv);
	if (!init_sim(sim))
		return (free_sim(sim), 0);
	return (preparatives(sim), free_sim(sim), 0);
}
