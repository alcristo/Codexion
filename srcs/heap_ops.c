/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:29:04 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:24:28 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_up(t_heap *heap, int index)
{
	int	parent;

	if (!index)
		return ;
	while (1)
	{
		parent = (index - 1) / 2;
		if (heap_before(heap->nodes[index], heap->nodes[parent], heap->mode))
			swap_requests(heap->nodes[index], heap->nodes[parent]);
		else
			break ;
		index = parent;
		if (!index)
			break ;
	}
}

void	heap_down(t_heap *heap, int index)
{
	int	child1;
	int	child2;
	int	best;

	if (!heap->size)
		return ;
	while (1)
	{
		best = index;
		child1 = 2 * index + 1;
		child2 = 2 * index + 2;
		if (child1 < heap->size
			&& heap_before(heap->nodes[child1], heap->nodes[best], heap->mode))
			best = child1;
		if (child2 < heap->size
			&& heap_before(heap->nodes[child2], heap->nodes[best], heap->mode))
			best = child2;
		if (best == index)
			break ;
		swap_requests(heap->nodes[index], heap->nodes[best]);
		index = best;
	}
}

int	enqueue(t_heap *heap, t_coder *coder, int requests)
{
	t_node	*new;

	pthread_mutex_lock(&heap->mutex);
	while (heap->size == heap->capacity && !sim_should_stop(heap->sim))
		pthread_cond_wait(&heap->cond, &heap->mutex);
	if (sim_should_stop(heap->sim))
		return (pthread_mutex_unlock(&heap->mutex), 1);
	new = heap->nodes[heap->size];
	pthread_mutex_lock(&coder->mutex);
	new->coder = coder;
	new->times = coder->times;
	new->deadline = coder->deadline;
	pthread_mutex_unlock(&coder->mutex);
	new->request_id = requests;
	new->order = heap->total_requests;
	heap->total_requests++;
	heap->size++;
	heap_up(heap, heap->size - 1);
	pthread_cond_broadcast(&heap->cond);
	pthread_mutex_unlock(&heap->mutex);
	return (0);
}

void	dequeue(t_heap *heap)
{
	//pthread_mutex_lock(&heap->mutex);
	if (heap->size == 0)
	{
		//pthread_mutex_unlock(&heap->mutex);
		return ;
	}
	heap->size--;
	if (heap->size > 0)
	{
		swap_requests(heap->nodes[0], heap->nodes[heap->size]);
		heap_down(heap, 0);
	}
	pthread_cond_broadcast(&heap->cond);
	//pthread_mutex_unlock(&heap->mutex);
}
