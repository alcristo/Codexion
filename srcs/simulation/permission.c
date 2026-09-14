/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   permission.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:45:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 16:09:10 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	fifo(t_sim *sim, t_heap_node *first) {
	int	i;
	int	n;

	i = first->coder->id;
	n = sim->params->num;
	if (!sim->coder[(i - 2) % n]->right && !sim->coder[i % n]->left)
	{
		if (!sim->dongles[i % n]->cool && !sim->dongles[(i + 1) % n]->cool)
			coder->permission == 1;
	}
	dequeue(sim->heap);
	pthread_broadcast(first->coder->wait);
}

void	edf(t_sim *sim, t_heap_node *first) {
	int	i;
	int	n;

	i = first->coder->id;
	n = sim->params->num;
	if (sim->coder[(i - 2) % n]->right || sim->coder[i % n]->left)
	{
		if (first->deadline != 0)
			return ;
	}
	else
	{
		if (!sim->dongles[i % n]->cool && !sim->dongles[(i + 1) % n]->cool)
			coder->permission == 1;
	}
	dequeue(sim->heap);
	pthread_broadcast(first->coder->wait);
}

void	shedule(t_sim *sim, t_heap_node *first) {
	int			i;
	int			n;

	i = first->coder->id;
	n = sim->params->num;
	if (sim->mode == FIFO)
	{
		if (!sim->coder[(i - 2) % n]->right && !sim->coder[i % n]->left)
		{
			coder->permission == 1;
			pthread_broadcast(first->coder->wait);
		}
		dequeue(sim->heap);
	}
	else if (sim->mode == EDF)
	{
		if (sim->coder[(i - 2) % n]->right || sim->coder[i % n]->left)
		{
			if (first->deadline != 0)
				return ;
		}
		else
		{
			coder->permission == 1;
			pthread_broadcast(first->coder->wait);
		}
		dequeue(sim->heap);
	}
}

void	attend_request(t_sim *sim) {
	t_heap_node	*first;

	if (sim->stop || !sim->heap->size)
		return ;
	pthread_mutex_lock(sim->heap->mutex);
	first = sim->heap->nodes[0];
	pthread_mutex_unlock(sim->heap->mutex);
	//schedule(sim, first);
	if (sim->mode == FIFO)
		fifo(sim, first);
	else if (sim->mode == EDF)
		edf(sim, first);
}
