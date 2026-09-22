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

int	sim_should_stop(t_sim *sim)
{
	int	stop;

	pthread_mutex_lock(sim->mutex);
	stop = sim->stop;
	pthread_mutex_unlock(sim->mutex);
	return (stop);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(coder->sim->mutex);
	while (!coder->sim->started && !coder->sim->stop)
		pthread_cond_wait(coder->sim->start, coder->sim->mutex);
	pthread_mutex_unlock(coder->sim->mutex);
	if (sim_should_stop(coder->sim))
		return (NULL);
	while (!sim_should_stop(coder->sim))
	{
		if (compile(coder, coder->sim->dongles))
			continue ;
		if (sim_should_stop(coder->sim))
			return (NULL);
		debug(coder);
		if (sim_should_stop(coder->sim))
			return (NULL);
		refactor(coder);
	}
	return (NULL);
}

void	*logger_routine(void *arg)
{
	t_sim		*sim;
	t_logger	*logger;
	int			finished;

	sim = (t_sim *)arg;
	logger = sim->logger;
	pthread_mutex_lock(sim->mutex);
	while (!sim->started && !sim->stop)
		pthread_cond_wait(sim->start, sim->mutex);
	pthread_mutex_unlock(sim->mutex);
	while (1)
	{
		pthread_mutex_lock(logger->mutex);
		if (logger->logs)
		{
			pthread_mutex_unlock(logger->mutex);
			print_log(logger);
			continue ;
		}
		pthread_mutex_unlock(logger->mutex);
		pthread_mutex_lock(sim->mutex);
		finished = sim->finished;
		pthread_mutex_unlock(sim->mutex);
		if (finished)
			break ;
		pthread_mutex_lock(logger->mutex);
		if (!logger->logs)
			pthread_cond_wait(logger->wait, logger->mutex);
		pthread_mutex_unlock(logger->mutex);
	}
	while (print_log(logger))
		;
	return (NULL);
}

void	tell_to_stop(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(sim->mutex);
	sim->stop = 1;
	//pthread_cond_broadcast(sim->start);
	pthread_mutex_unlock(sim->mutex);
	i = 0;
	while (i < sim->params->num)
	{
		pthread_mutex_lock(sim->coders[i]->go);
		pthread_cond_broadcast(sim->coders[i]->wait);
		pthread_mutex_unlock(sim->coders[i]->go);
		i++;
	}
	pthread_mutex_lock(sim->logger->mutex);
	sim->logger->silence = 1;
	pthread_cond_broadcast(sim->logger->wait);
	pthread_mutex_unlock(sim->logger->mutex);
}

void	director_routine(t_sim *sim)
{
	int	i;
	int	done;

	while (!sim_should_stop(sim))
	{
		i = 0;
		done = 1;
		while (i < sim->params->num)
		{
			pthread_mutex_lock(sim->coders[i]->mutex);
			if (sim->coders[i]->status == BURNED_OUT)
				return (pthread_mutex_unlock(sim->coders[i]->mutex),
					tell_to_stop(sim));
			if (sim->coders[i]->times < sim->params->required)
				done = 0;
			pthread_mutex_unlock(sim->coders[i]->mutex);
			i++;
		}
		if (done)
			return (tell_to_stop(sim));
		cool_dongles(sim);
		pthread_mutex_lock(sim->heap->mutex);
		while (!sim->stop && !sim->heap->size)
			pthread_cond_wait(sim->request_wait, sim->heap->mutex);
		//pthread_mutex_unlock(sim->mutex);
		if (sim_should_stop(sim))
		{
			pthread_mutex_unlock(sim->heap->mutex);
			return ;
		}
		//pthread_mutex_lock(sim->heap->mutex);
		attend_request(sim);
		pthread_mutex_unlock(sim->heap->mutex);
	}
}

pthread_t	*create_threads(t_sim *sim)
{
	int			i;
	pthread_t	*coders;

	i = 0;
	coders = malloc(sim->params->num * sizeof(pthread_t));
	if (!coders)
		return (NULL);
	memset(coders, 0, sim->params->num * sizeof(pthread_t));
	pthread_mutex_lock(sim->mutex);
	while (i < sim->params->num)
	{
		if (pthread_create(&coders[i], NULL, coder_routine, sim->coders[i]))
		{
			sim->stop = 1;
			pthread_cond_broadcast(sim->start);
			while (i > 0)
				pthread_join(coders[--i], NULL);
			free(coders);
			return (pthread_mutex_unlock(sim->mutex), NULL);
		}
		i++;
	}
	return (pthread_mutex_unlock(sim->mutex), coders);
}

static void	initial_requests(t_sim *sim)
{
	int	i;

	i = 0;
	pthread_mutex_lock(sim->heap->mutex);
	while (i < sim->params->num)
	{
		enqueue(sim->heap, sim->coders[i]);
		i++;
	}
	/*i = 0;
	while (i < (int)sim->heap->size)
		printf("%d\n", sim->heap->nodes[i++]->coder->id);*/
	pthread_mutex_unlock(sim->heap->mutex);
}

void	preparatives(t_sim *sim)
{
	pthread_t	*coders;
	pthread_t	logger;
	size_t		i;

	if (pthread_create(&logger, NULL, logger_routine, sim))
		return ;
	coders = create_threads(sim);
	if (!coders)
	{
		pthread_mutex_lock(sim->mutex);
		sim->stop = 1;
		pthread_cond_broadcast(sim->start);
		pthread_mutex_unlock(sim->mutex);
		pthread_join(logger, NULL);
		return ;
	}
	i = 0;
	initial_requests(sim);
	pthread_mutex_lock(sim->mutex);
	sim->start_time = now();
	if (sim->start_time < 0)
	{
		sim->stop = 1;
		pthread_cond_broadcast(sim->start);
		pthread_mutex_unlock(sim->mutex);
		while (i < (size_t)sim->params->num)
			pthread_join(coders[i++], NULL);
		pthread_join(logger, NULL);
		free(coders);
		return ;
	}
	sim->last_cool = sim->start_time;
	sim->started = 1;
	pthread_cond_broadcast(sim->start);
	pthread_mutex_unlock(sim->mutex);
	director_routine(sim);
	while (i < (size_t)sim->params->num)
		pthread_join(coders[i++], NULL);
	pthread_mutex_lock(sim->mutex);
	sim->finished = 1;
	pthread_mutex_unlock(sim->mutex);
	pthread_mutex_lock(sim->logger->mutex);
	pthread_cond_broadcast(sim->logger->wait);
	pthread_mutex_unlock(sim->logger->mutex);
	pthread_join(logger, NULL);
	free(coders);
}
