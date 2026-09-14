/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:33:13 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 15:54:35 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	coder_routine(t_coder *coder) {
	pthread_cond_wait(coder->sim->start)
	while (!coder->sim->stop)
	{
		if compile(coder, coder->sim->dongles)
		{
			coder->status = BURNED_OUT;
			return ;
		}
		if (coder->sim->stop)
			return ;
		debug(coder);
		refactor(coder);
	}
}

void	director_routine(t_sim *sim) {
	size_t	i;
	size_t	done;

	while (!sim->stop)
	{
		i = 0;
		done = 1;
		while (i++ < sim->params->num)
		{
			if (sim->coders[i]->status == BURNED_OUT)
			{
				sim->stop = 1;
				return ;
			}
			if (sim->coders[i]->times < sim->params->required)
				done = 0;
		}
		if (done)
			sim->stop = 1;
		cool_dongles(sim->dongles)
		attend_request(sim);
		ft_sleep(50);
	}
}

void	preparatives(t_sim *sim) {
	pthread_t	coders[sim->params->num];
	size_t		i;

	i = 0;
	while (i++ < sim->params->num)
	{
		pthread_create(coders[i], NULL, coder_routine, sim->coders[i]);
		if (!coders[i])
		{
			sim->stop = 1;
			while (--i >= 0)
				pthread_join(coders[i], NULL);
			free_sim(sim);
		}
	}
	i = 0;
	pthread_broadcast(start);
	sim->start_time = gettimeofday();
	sim->last_cool = sim->start_time;
	director_routine(sim);
	while (i++ < sim->params->num)
		pthread_join(coders[i], NULL);
	free_sim(sim);
}
