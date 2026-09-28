/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 15:28:09 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:39:59 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	pthread_mutex_lock(&coder->sim->mutex);
	while (!coder->sim->started && !coder->sim->stop)
		pthread_cond_wait(&coder->sim->cond, &coder->sim->mutex);
	pthread_mutex_unlock(&coder->sim->mutex);
	//if (send_request(coder))
	//	return (NULL);
	while (!sim_should_stop(coder->sim))
	{
		if (send_request(coder))
			break ;
		if (compile(coder))
			continue ;
		if (sim_should_stop(coder->sim))
			break ;
		program(coder);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->mutex);
	while (!sim->started && !sim->stop)
		pthread_cond_wait(&sim->cond, &sim->mutex);
	pthread_mutex_unlock(&sim->mutex);
	while (!sim_should_stop(sim))
	{
		if (check_status(sim->coders))
		{
			tell_to_stop(sim);
			break ;
		}
		usleep(50);
	}
	return (NULL);
}

void	*waiter_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	pthread_mutex_lock(&sim->mutex);
	while (!sim->started && !sim->stop)
		pthread_cond_wait(&sim->cond, &sim->mutex);
	pthread_mutex_unlock(&sim->mutex);
	if (sim->number == 1)
		return (NULL);
	usleep(100);
	while (!sim_should_stop(sim))
	{
		pthread_mutex_lock(&sim->heap->mutex);
		while (!sim->heap->size && !sim_should_stop(sim))
			pthread_cond_wait(&sim->heap->cond, &sim->heap->mutex);
		pthread_mutex_unlock(&sim->heap->mutex);
		if (sim_should_stop(sim))
			break ;
		attend_request(sim);
	}
	return (NULL);
}

void	*logger_routine(void *arg)
{
	t_logger	*logger;

	logger = (t_logger *)arg;
	pthread_mutex_lock(&logger->sim->mutex);
	while (!logger->sim->started && !logger->sim->stop)
		pthread_cond_wait(&logger->sim->cond, &logger->sim->mutex);
	pthread_mutex_unlock(&logger->sim->mutex);
	while (1)
	{
		pthread_mutex_lock(&logger->mutex);
		while (!logger->logs && !sim_should_stop(logger->sim))
			pthread_cond_wait(&logger->cond, &logger->mutex);
		if (!logger->logs && sim_should_stop(logger->sim))
		{
			pthread_mutex_unlock(&logger->mutex);
			break ;
		}
		pthread_mutex_unlock(&logger->mutex);
		print_log(logger);
	}
	clean_logs(logger);
	return (NULL);
}
