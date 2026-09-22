/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 16:28:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 11:41:28 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	heap_before(t_heap_node *a, t_heap_node *b)
{
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (a->times == 0 && b->times == 0)
	{
		if ((a->coder->id % 2) != (b->coder->id % 2))
			return (a->coder->id % 2 == 0);
	}
	return (a->coder->id < b->coder->id);
}

void	ft_swap(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	heap_up(t_heap *heap, size_t index)
{
	size_t	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!heap_before(heap->nodes[index], heap->nodes[parent]))
			break ;
		ft_swap(heap->nodes[parent], heap->nodes[index]);
		index = parent;
	}
}

void	heap_down(t_heap *heap, size_t index)
{
	size_t	son1;
	size_t	son2;
	size_t	best;

	while (1)
	{
		son1 = 2 * index + 1;
		son2 = 2 * index + 2;
		best = index;
		if (son1 < heap->size
			&& heap_before(heap->nodes[son1], heap->nodes[best]))
			best = son1;
		if (son2 < heap->size
			&& heap_before(heap->nodes[son2], heap->nodes[best]))
			best = son2;
		if (best == index)
			break ;
		ft_swap(heap->nodes[index], heap->nodes[best]);
		index = best;
	}
}

void	enqueue(t_heap *heap, t_coder *coder)
{
	size_t		size;

	if (heap->size == heap->capacity)
		return ;
	size = heap->size;
	heap->nodes[size]->coder = coder;
	pthread_mutex_lock(coder->mutex);
	heap->nodes[size]->times = coder->times;
	if (coder->times == 0)
		heap->nodes[size]->deadline = coder->sim->params->time_burnout;
	else
		heap->nodes[size]->deadline = coder->last_compile
			+ coder->sim->params->time_burnout
			+ coder->sim->params->time_compile;
	pthread_mutex_unlock(coder->mutex);
	heap->size++;
	if (heap->mode == EDF)
		heap_up(heap, heap->size - 1);
}

void	dequeue(t_heap *heap)
{
	if (heap->size == 0)
		return ;
	heap->size--;
	if (heap->size > 0)
	{
		ft_swap(heap->nodes[0], heap->nodes[heap->size]);
		if (heap->mode == EDF)
			heap_down(heap, 0);
	}
}
