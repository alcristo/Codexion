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
	if (sim->coders)
		free_coders(sim->coders);
	if (sim->dongles)
		free_dongles(sim->dongles);
	if (sim->heap)
		free_heap(sim->heap);
	if (sim->logger)
		free_logger(sim->logger);
	pthread_mutex_destroy(&sim->mutex);
	//pthread_mutex_destroy(&sim->resource_mutex);
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
	if (pthread_mutex_init(&sim->mutex, NULL))
		return (0);
	//if (pthread_mutex_init(&sim->resource_mutex, NULL))
	//	return (0);
	if (pthread_cond_init(&sim->cond, NULL))
		return (0);
	sim->coders = create_coders(sim);
	if (!sim->coders)
		return (0);
	sim->dongles = create_dongles(sim);
	if (!sim->dongles)
		return (0);
	sim->heap = create_heap(sim);
	if (!sim->heap)
		return (0);
	sim->logger = create_logger(sim);
	if (!sim->logger)
		return (0);
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
