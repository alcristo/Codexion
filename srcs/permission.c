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

static void	permission_granted(t_sim *sim, int i)
{
	t_coder	*coder;
	int		n;

	n = sim->number;
	coder = sim->coders[i];
	sim->dongles[i]->reserved = 1;
	pthread_mutex_unlock(&sim->dongles[i]->mutex);
	sim->dongles[(i + 1) % n]->reserved = 1;
	pthread_mutex_unlock(&sim->dongles[(i + 1) % n]->mutex);
	pthread_mutex_lock(&coder->go);
	coder->permission = 1;
	pthread_cond_broadcast(&coder->cond);
	pthread_mutex_unlock(&coder->go);
	dequeue(sim->heap);
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

void	attend_request(t_sim *sim)
{
	t_node	*first;
	int		i;
	int		n;

	if (sim->number == 1)
	{
		pthread_mutex_lock(&sim->heap->mutex);
		pthread_cond_broadcast(&sim->heap->cond);
		pthread_mutex_unlock(&sim->heap->mutex);
		return ;
	}
	pthread_mutex_lock(&sim->heap->mutex);
	if (!sim->heap->size)
	{
		pthread_mutex_unlock(&sim->heap->mutex);
		return ;
	}
	first = sim->heap->nodes[0];
	n = sim->number;
	i = (n + first->coder->id - 1) % n;
	pthread_mutex_unlock(&sim->heap->mutex);
	lock_order(sim->dongles, i, n);
	if (!check_dongles(sim, i))
		return (permission_granted(sim, i));
	pthread_mutex_unlock(&sim->dongles[i]->mutex);
	pthread_mutex_unlock(&sim->dongles[(i + 1) % n]->mutex);
}
