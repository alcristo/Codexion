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

#include "codexion.h"

void	send_request(t_coder *coder) {
	t_heap	*heap;
	int		i;

	pthread_mutex_lock(coder->sim->heap->mutex);
	heap = coder->sim->heap;
	pthread_mutex_unlock(coder->sim->heap->mutex);
	i = 0;
	while (i++ < heap->size)
	{
		if (heap->nodes[i]->coder->id == coder->id)
			return ;
	}
	pthread_mutex_lock(coder->sim->heap->mutex);
	enqueue(heap, coder);
	pthread_mutex_unlock(coder->sim->heap->mutex);
}

int	compile(t_coder *coder, t_dongle **dongles) {
	size_t	i;
	int		n;
	long	t;

	i = coder->id;
	n = coder->sim->params->num;
	send_request(coder);
	pthread_cond_timedwait(wait, go, coder->sim->params->time_burnout);
	if (!coder->permission)
		return (compile(coder, dongles));
	coder->status++;
	grab_dongles(coder, dongles);
	t = gettimeofday() - coder->sim->start_time;
	coder->last_compile = t;
	send_log(coder, "compile", t);
	ft_sleep(coder->sim->params->time_compile);
	release_dongles(coder, dongles);
	coder->times++;
	coder->status++;
	return (0);
}

void	debug(size_t i, t_coder *coder) {
	long	t;

	t = gettimeofday() - coder->sim->start_time;
	send_log(coder, "debug", t);
	ft_sleep(coder->sim->time_debug);
	coder->status++;
}

void	refactor(size_t i, t_coder *coder) {
	long	t;

	t = gettimeofday() - coder->sim->start_time;
	send_log(coder, "refactor", t);
	ft_sleep(coder->sim->time_refactor);
	coder->status = IDLE;
}
