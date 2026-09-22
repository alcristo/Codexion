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
	int	left;
	int	right;
	int	n;

	n = sim->params->num;
	left = coder->id - 1;
	right = (left + 1) % n;
	sim->dongles[left]->owned = 1;
	sim->dongles[right]->owned = 1;
	dequeue(sim->heap);
	pthread_mutex_unlock(sim->dongles[left]->mutex);
	pthread_mutex_unlock(sim->dongles[right]->mutex);
	pthread_mutex_lock(coder->go);
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

static void	grant_permission(t_sim *sim, t_coder *coder)
{
	int	n;
	int	left;
	int	right;
	//int	permission;

	n = sim->params->num;
	if (n == 1)
		return ;
	left = coder->id - 1;
	right = (left + 1) % n;
	check_dongles(sim->dongles, left, n);
	if (!sim->dongles[left]->owned && !sim->dongles[right]->owned)
	{
		if (!sim->dongles[left]->cool && !sim->dongles[right]->cool)
			return (permission_accepted(sim, coder));
	}
	/*if (!sim->dongles[left]->owned && !sim->dongles[left]->cool)
	{
		if (!sim->dongles[right]->owned && !sim->dongles[right]->cool)
			permission = 1;
		else
			permission = 0;
	}
	else
		permission = 0;
	if (permission)
		return (permission_accepted(sim, sim->coders[i]));*/
	pthread_mutex_unlock(sim->dongles[left]->mutex);
	pthread_mutex_unlock(sim->dongles[right]->mutex);
}

void	attend_request(t_sim *sim)
{
	t_heap_node	*first;
	int			i;

	//pthread_mutex_lock(sim->mutex);
	if (sim->stop || !sim->heap->size)
	{
	//	pthread_mutex_unlock(sim->mutex);
		return ;
	}
	//pthread_mutex_unlock(sim->mutex);
	first = sim->heap->nodes[0];
	i = first->coder->id - 1;
	grant_permission(sim, first->coder);
}
