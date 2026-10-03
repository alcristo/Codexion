/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparatives.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 13:29:00 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:42:14 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_threads(t_threads *threads, t_sim *sim)
{
	int	i;

	if (pthread_create(&threads->monitor, NULL, monitor_routine, sim))
		return (1);
	if (pthread_create(&threads->waiter, NULL, waiter_routine, sim))
		return (1);
	i = 0;
	while (i < sim->number)
	{
		if (pthread_create(
				&threads->coders[i], NULL, coder_routine, sim->coders[i]))
			return (i);
		i++;
	}
	return (i);
}

void	threads_failure(t_sim *sim)
{
	pthread_mutex_lock(&sim->mutex);
	sim->stop = 1;
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->mutex);
}

void	start(t_sim *sim)
{
	struct timeval	tv;
	int				i;

	pthread_mutex_lock(&sim->mutex);
	if (gettimeofday(&tv, NULL))
		return (pthread_mutex_unlock(&sim->mutex), threads_failure(sim));
	i = 0;
	sim->started++;
	sim->start_time = (long)(tv.tv_sec * 1000000L + tv.tv_usec);
	pthread_cond_broadcast(&sim->cond);
	pthread_mutex_unlock(&sim->mutex);
}

void	join_threads(t_threads *threads, int n)
{
	int	i;

	pthread_join(threads->monitor, NULL);
	pthread_join(threads->waiter, NULL);
	i = 0;
	while (i < n)
		pthread_join(threads->coders[i++], NULL);
}

void	preparatives(t_sim *sim)
{
	t_threads	*threads;
	int			init;

	threads = malloc(sizeof(t_threads));
	if (!threads)
		return ;
	memset(threads, 0, sizeof(t_threads));
	threads->coders = malloc(sim->number * sizeof(pthread_t));
	if (!threads->coders)
		return (free(threads));
	init = start_threads(threads, sim);
	if (init < sim->number)
		threads_failure(sim);
	else
		start(sim);
	join_threads(threads, init);
	free(threads->coders);
	free(threads);
}
