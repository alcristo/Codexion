/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 10:52:17 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 15:17:57 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	send_request(t_coder *coder)
{
	size_t	i;
	t_heap	*heap;

	i = 0;
	heap = coder->sim->heap;
	pthread_mutex_lock(heap->mutex);
	while (i < coder->sim->heap->size)
	{
		if (heap->nodes[i]->coder->id == coder->id)
		{
			pthread_mutex_unlock(heap->mutex);
			return ;
		}
		i++;
	}
	enqueue(heap, coder);
	pthread_cond_broadcast(coder->sim->request_wait);
	pthread_mutex_unlock(heap->mutex);
}

int	wait(t_coder *coder)
{
	struct timespec	time;
	long			deadline;
	int				ret;

	pthread_mutex_lock(coder->sim->mutex);
	pthread_mutex_lock(coder->mutex);
	deadline = coder->sim->start_time + coder->last_compile
		+ coder->sim->params->time_burnout;
	pthread_mutex_unlock(coder->mutex);
	pthread_mutex_unlock(coder->sim->mutex);
	time.tv_sec = deadline / 1000;
	time.tv_nsec = (deadline % 1000) * 1000000L;
	pthread_mutex_lock(coder->go);
	while (!sim_should_stop(coder->sim) && !coder->permission)
	{
		ret = pthread_cond_timedwait(coder->wait, coder->go, &time);
		if (coder->permission || sim_should_stop(coder->sim))
			break ;
		if (ret)
			return (pthread_mutex_unlock(coder->go), 1);
	}
	if (sim_should_stop(coder->sim))
		return (pthread_mutex_unlock(coder->go), 1);
	return (pthread_mutex_unlock(coder->go), 0);
}

void	burnout(t_coder *coder)
{
	long	time;
	long	deadline;

	pthread_mutex_lock(coder->mood);
	coder->status = BURNED_OUT;
	pthread_mutex_unlock(coder->mood);
	time = now() - coder->sim->start_time;
	pthread_mutex_lock(coder->mutex);
	deadline = coder->sim->start_time + coder->last_compile
		+ coder->sim->params->time_burnout;
	pthread_mutex_unlock(coder->mutex);
	send_burnout_log(coder, time, deadline);
}

int	compile(t_coder *coder, t_dongle **dongles)
{
	size_t	i;
	int		n;
	long	t;

	i = coder->id;
	n = coder->sim->params->num;
	if (coder->times)
		send_request(coder);
	if (wait(coder))
	{
		if (now() - coder->sim->start_time - coder->last_compile
			>= coder->sim->params->time_burnout)
			return (burnout(coder), 1);
	}
	pthread_mutex_lock(coder->go);
	if (!coder->permission)
		return (pthread_mutex_unlock(coder->go), 1);
	coder->permission = 0;
	pthread_mutex_unlock(coder->go);
	grab_dongles(coder, dongles);
	pthread_mutex_lock(coder->mood);
	coder->status++;
	pthread_mutex_unlock(coder->mood);
	if (coder->left && coder->right)
	{
		t = now() - coder->sim->start_time;
		pthread_mutex_lock(coder->mutex);
		coder->last_compile = t;
		pthread_mutex_unlock(coder->mutex);
		send_log(coder, LOG_COMPILE, t);
		ft_sleep(coder->sim->params->time_compile);
	}
	release_dongles(coder);
	pthread_mutex_lock(coder->mutex);
	coder->times++;
	pthread_mutex_unlock(coder->mutex);
	pthread_mutex_lock(coder->mood);
	coder->status++;
	pthread_mutex_unlock(coder->mood);
	return (0);
}

void	debug(t_coder *coder)
{
	long	t;

	t = now() - coder->sim->start_time;
	send_log(coder, LOG_DEBUG, t);
	ft_sleep(coder->sim->params->time_debug);
	pthread_mutex_lock(coder->mood);
	coder->status++;
	pthread_mutex_unlock(coder->mood);
}

void	refactor(t_coder *coder)
{
	long	t;

	t = now() - coder->sim->start_time;
	send_log(coder, LOG_REFACTOR, t);
	ft_sleep(coder->sim->params->time_refactor);
	pthread_mutex_lock(coder->mood);
	coder->status = IDLE;
	pthread_mutex_unlock(coder->mood);
}
