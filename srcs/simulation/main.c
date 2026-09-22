/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:51:42 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 12:10:23 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_sim(t_sim *sim)
{
	if (sim->coders)
		free_coders(sim->coders, sim->params->num);
	if (sim->dongles)
		free_dongles(sim->dongles, sim->params->num);
	if (sim->logger)
		free_logger(sim->logger);
	if (sim->heap)
		free_heap(sim->heap);
	if (sim->mutex)
	{
		pthread_mutex_destroy(sim->mutex);
		free(sim->mutex);
	}
	if (sim->start)
	{
		pthread_cond_destroy(sim->start);
		free(sim->start);
	}
	if (sim->params)
		free(sim->params);
	free(sim);
}

static void	sim_init_ints(t_sim *sim)
{
	sim->started = 0;
	sim->stop = 0;
	sim->start_time = 0;
	sim->last_cool = 0;
	sim->finished = 0;
}

static int	sim_init(t_sim *sim)
{
	sim_init_ints(sim);
	sim->coders = create_coders(sim);
	if (!sim->coders)
		return (1);
	sim->dongles = create_dongles(sim->params);
	if (!sim->dongles)
		return (1);
	sim->logger = create_logger(sim);
	if (!sim->logger)
		return (1);
	sim->heap = create_heap(sim);
	if (!sim->heap)
		return (1);
	sim->mutex = malloc(sizeof(pthread_mutex_t));
	if (!sim->mutex)
		return (1);
	pthread_mutex_init(sim->mutex, NULL);
	sim->start = malloc(sizeof(pthread_cond_t));
	if (!sim->start)
		return (1);
	pthread_cond_init(sim->start, NULL);
	sim->request_wait = malloc(sizeof(pthread_cond_t));
	if (!sim->request_wait)
		return (1);
	pthread_cond_init(sim->request_wait, NULL);
	return (0);
}

int	main(int argc, char **argv)
{
	t_sim	*sim;

	if (parse_args(argc, argv))
		return (0);
	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (0);
	memset(sim, 0, sizeof(t_sim));
	sim->params = parse_params(argv);
	if (!sim->params)
		return (free_sim(sim), 0);
	if (sim_init(sim))
		return (free_sim(sim), 0);
	return (preparatives(sim), free_sim(sim), 0);
}
