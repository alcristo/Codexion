/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:29:34 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/21 11:40:27 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	even(t_heap *heap)
{
	size_t	i;

	i = 1;
	while (i < heap->size)
	{
		if (heap->nodes[i]->coder->id % 2 == heap->sim->last_permission % 2
			&& !heap->nodes[i]->coder->times)
			return (ft_swap(heap->nodes[0], heap->nodes[i]),
				attend_request(heap->sim));
		i++;
	}
	if (heap->sim->first_routine)
	{
		pthread_mutex_lock(heap->sim->mutex);
		heap->sim->first_routine = 1;
		pthread_mutex_unlock(heap->sim->mutex);
	}
	return (tiebreaker(heap));
}

void	odd(t_heap *heap)
{
	size_t	i;

	i = 1;
	while (i < heap->size)
	{
		if (!heap->nodes[i]->coder->id % 2 && !heap->nodes[i]->coder->times)
			return (ft_swap(heap->nodes[0], heap->nodes[i]),
				attend_request(heap->sim));
		i++;
	}
	if (heap->sim->first_routine)
	{
		pthread_mutex_lock(heap->sim->mutex);
		heap->sim->first_routine = 1;
		pthread_mutex_unlock(heap->sim->mutex);
	}
	return (tiebreaker(heap));
}

void	tiebreaker(t_heap *heap)
{
	size_t	i;
	int		id;
	int		go;
	int		n;

	i = 0;
	n = heap->sim->params->num;
	go = 0;
	while (i < heap->size - 1
		&& heap->nodes[i]->deadline == heap->nodes[i + 1]->deadline)
	{
		id = heap->nodes[i]->coder->id - 1;
		check_dongles(heap->sim->dongles, id, n);
		if (!heap->sim->dongles[id]->owned
			&& !heap->sim->dongles[id]->cool
			&& !heap->sim->dongles[(id + 1) % n]->owned
			&& !heap->sim->dongles[(id + 1) % n]->cool)
			go = 1;
		pthread_mutex_unlock(heap->sim->dongles[id]->mutex);
		pthread_mutex_unlock(heap->sim->dongles[(id + 1) % n]->mutex);
		if (go)
			return (ft_swap(heap->nodes[0], heap->nodes[i]));
		i++;
	}
}

void	tie_break(t_heap *heap)
{
	t_heap_node	*first;

	if (heap->sim->first_routine)
		return (tiebreaker(heap));
	first = heap->nodes[0];
	if (first->coder->times == 0)
	{
		if (heap->sim->params->num % 2)
			return (odd(heap));
		else
			return (even(heap));
	}
	else
	{
		if (!heap->sim->first_routine)
			heap->sim->first_routine = 0;
		return (tiebreaker(heap));
	}
}
