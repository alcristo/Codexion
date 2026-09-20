/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permission.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:45:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 11:34:48 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static void	permission_accepted(t_sim *sim, t_coder *coder)
{
	int	i;
	int	n;

	i = coder->id - 1;
	n = sim->params->num;
	dequeue(sim->heap);
	pthread_mutex_lock(sim->dongles[i]->mutex);
	sim->dongles[i]->owned = 1;
	pthread_mutex_unlock(sim->dongles[i]->mutex);
	pthread_mutex_lock(sim->dongles[(i + 1) % n]->mutex);
	sim->dongles[(i + 1) % n]->owned = 1;
	pthread_mutex_unlock(sim->dongles[(i + 1) % n]->mutex);
	pthread_mutex_lock(coder->go);
	//sim->last_permission = coder->id;
	coder->permission = 1;
	pthread_cond_broadcast(coder->wait);
	pthread_mutex_unlock(coder->go);
}

void	check_dongles(t_dongle **dongles, int i, int n)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % n;

	if (left < right)
	{
		pthread_mutex_lock(dongles[left]->mutex);
		pthread_mutex_lock(dongles[right]->mutex);
	}
	else
	{
		pthread_mutex_lock(dongles[right]->mutex);
		pthread_mutex_lock(dongles[left]->mutex);
	}
}

static int	grant_permission(t_sim *sim, int i)
{
	int	n;
	int	left;
	int	right;
	int	permission;

	n = sim->params->num;
	if (n == 1)
		return (1);
	left = i;
	right = (i + 1) % n;
	check_dongles(sim->dongles, i, n);
	if (!sim->dongles[left]->owned && !sim->dongles[left]->cool)
	{
		if (!sim->dongles[right]->owned && !sim->dongles[right]->cool)
			permission = 1;
		else
			permission = 0;
	}
	else
		permission = 0;
	pthread_mutex_unlock(sim->dongles[left]->mutex);
	pthread_mutex_unlock(sim->dongles[right]->mutex);
	if (permission)
		return (permission_accepted(sim, sim->coders[i]), 0);
	return (1);
}

void	attend_request(t_sim *sim)
{
	t_heap_node	*first;
	int			i;
	int			n;

	pthread_mutex_lock(sim->mutex);
	if (sim->stop || !sim->heap->size)
	{
		pthread_mutex_unlock(sim->mutex);
		return ;
	}
	pthread_mutex_unlock(sim->mutex);
	first = sim->heap->nodes[0];
	i = first->coder->id - 1;
	n = sim->params->num;
	if (grant_permission(sim, i) && sim->heap->size > 1)
	{
		if (first->deadline == sim->heap->nodes[1]->deadline
			|| first->deadline == sim->heap->nodes[2]->deadline)
		{
			if (sim->heap->mode == EDF)
				tiebreaker(sim->heap);
		}
	}
}
