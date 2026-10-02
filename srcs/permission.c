/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permission.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 11:17:43 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:49:35 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	permission_granted(t_sim *sim, t_coder *coder)
{
	int		i;
	int		n;
	long	deadline;

	n = sim->number;
	i = (n + coder->id - 1) % n;
	sim->dongles[i]->reserved = 1;
	sim->dongles[(i + 1) % n]->reserved = 1;
	pthread_mutex_unlock(&sim->dongles[i]->mutex);
	pthread_mutex_unlock(&sim->dongles[(i + 1) % n]->mutex);
	pthread_mutex_lock(&coder->mutex);
	deadline = coder->deadline;
	coder->compiling = 1;
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->go);
	if (now() < deadline)
		coder->permission = 1;
	pthread_cond_broadcast(&coder->cond);
	pthread_mutex_unlock(&coder->go);
}

static int	check_dongles(t_sim *sim, int i)
{
	t_dongle	*left;
	t_dongle	*right;
	int			n;
	long		time;

	n = sim->number;
	left = sim->dongles[i];
	right = sim->dongles[(i + 1) % n];
	time = now();
	if (!left->reserved && !right->reserved)
	{
		if (time >= left->cooldown && time >= right->cooldown)
			return (0);
		else
			return (1);
	}
	else
		return (1);
}

static void	one_coder(t_sim *sim)
{
	pthread_mutex_lock(&sim->heap->mutex);
	pthread_cond_broadcast(&sim->heap->cond);
	pthread_mutex_unlock(&sim->heap->mutex);
}

void	attend_request(t_sim *sim)
{
	t_coder	*coder;
	int		i;
	int		n;
	int		req;

	if (sim->number == 1)
		return (one_coder(sim));
	pthread_mutex_lock(&sim->heap->mutex);
	if (!sim->heap->size)
	{
		pthread_mutex_unlock(&sim->heap->mutex);
		return ;
	}
	coder = sim->heap->nodes[0]->coder;
	req = sim->heap->nodes[0]->request_id;
	dequeue(sim->heap);
	pthread_mutex_unlock(&sim->heap->mutex);
	n = sim->number;
	i = (n + coder->id - 1) % n;
	lock_order(sim->dongles, i, n);
	if (!check_dongles(sim, i))
		return (permission_granted(sim, coder));
	pthread_mutex_unlock(&sim->dongles[i]->mutex);
	pthread_mutex_unlock(&sim->dongles[(i + 1) % n]->mutex);
	enqueue(sim->heap, coder, req + 1);
}
