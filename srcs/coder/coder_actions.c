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
	pthread_mutex_lock(coder->sim->heap->mutex);
	enqueue(coder->sim->heap, coder);
	pthread_mutex_unlock(coder->sim->heap->mutex);
}

int	wait(t_coder *coder)
{
	struct timespec	time;
	long			deadline;

	pthread_mutex_lock(coder->mutex);
	deadline = coder->sim->start_time + coder->last_compile
		+ coder->sim->params->time_burnout;
	pthread_mutex_unlock(coder->mutex);
	time.tv_sec = deadline / 1000;
	time.tv_nsec = (deadline % 1000) * 1000000L;
	pthread_mutex_lock(coder->go);
	while (!sim_should_stop(coder->sim) && !coder->permission)
	{
		if (pthread_cond_timedwait(coder->wait, coder->go, &time))
			return (pthread_mutex_unlock(coder->go), 1);
	}
	return (pthread_mutex_unlock(coder->go), 0);
}

static void	burnout(t_coder *coder)
{
	pthread_mutex_lock(coder->mutex);
	coder->status = BURNED_OUT;
	pthread_mutex_unlock(coder->mutex);
	send_log(coder, LOG_BURNOUT, now() - coder->sim->start_time);
	pthread_mutex_lock(coder->sim->mutex);
	coder->sim->stop = 1;
	pthread_mutex_unlock(coder->sim->mutex);
}

int	compile(t_coder *coder, t_dongle **dongles)
{
	size_t	i;
	int		n;
	long	t;

	i = coder->id;
	n = coder->sim->params->num;
	send_request(coder);
	if (wait(coder))
		return (burnout(coder), 1);
	pthread_mutex_lock(coder->go);
	if (!coder->permission)
		return (pthread_mutex_unlock(coder->go), 1);
	pthread_mutex_unlock(coder->go);
	grab_dongles(coder, dongles);
	pthread_mutex_lock(coder->mutex);
	coder->status++;
	t = now() - coder->sim->start_time;
	coder->last_compile = t;
	pthread_mutex_unlock(coder->mutex);
	if (coder->left && coder->right)
	{
		send_log(coder, LOG_COMPILE, t);
		ft_sleep(coder->sim->params->time_compile);
	}
	pthread_mutex_lock(coder->mutex);
	release_dongles(coder);
	coder->times++;
	coder->status++;
	pthread_mutex_unlock(coder->mutex);
	return (0);
}

void	debug(t_coder *coder)
{
	long	t;

	t = now() - coder->sim->start_time;
	send_log(coder, LOG_DEBUG, t);
	ft_sleep(coder->sim->params->time_debug);
	pthread_mutex_lock(coder->mutex);
	coder->status++;
	pthread_mutex_unlock(coder->mutex);
}

void	refactor(t_coder *coder)
{
	long	t;

	t = now() - coder->sim->start_time;
	send_log(coder, LOG_REFACTOR, t);
	ft_sleep(coder->sim->params->time_refactor);
	pthread_mutex_lock(coder->mutex);
	coder->status = IDLE;
	pthread_mutex_unlock(coder->mutex);
}
