/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:18:48 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:40:52 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_should_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->mutex);
	return (stop);
}

void	tell_to_stop(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->mutex);
	pthread_mutex_lock(&sim->heap->mutex);
	pthread_cond_broadcast(&sim->heap->cond);
	pthread_mutex_unlock(&sim->heap->mutex);
	i = 0;
	while (i < sim->number)
	{
		pthread_mutex_lock(&sim->coders[i]->go);
		pthread_cond_broadcast(&sim->coders[i]->cond);
		pthread_mutex_unlock(&sim->coders[i]->go);
		i++;
	}
}

void	ft_sleep(t_sim *sim, int t)
{
	long	time;

	time = now() + (long)t;
	while (!sim_should_stop(sim) && now() < time)
		usleep(1000);
}
