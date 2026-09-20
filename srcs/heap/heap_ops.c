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

void	ft_swap(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
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

void	next_request(t_heap *heap)
{
	size_t	i;

	i = 0;
	if (heap->nodes[0]->deadline != heap->nodes[1]->deadline
		&& heap->nodes[0]->deadline == heap->nodes[2]->deadline)
		return (ft_swap(heap->nodes[0], heap->nodes[2]));
	while (i < heap->size - 1
		&& heap->nodes[i]->deadline == heap->nodes[i + 1]->deadline)
	{
		ft_swap(heap->nodes[i], heap->nodes[i + 1]);
		i++;
	}
	heap_up(heap, i);
}

void	heap_up(t_heap *heap, size_t index)
{
	size_t	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->nodes[parent]->deadline <= heap->nodes[index]->deadline)
			break ;
		ft_swap(heap->nodes[parent], heap->nodes[index]);
		index = parent;
	}
}

void	heap_down(t_heap *heap, size_t index)
{
	size_t	son1;
	size_t	son2;
	size_t	min;

	while (1)
	{
		son1 = 2 * index + 1;
		son2 = 2 * index + 2;
		min = index;
		if (son1 < heap->size
			&& heap->nodes[son1]->deadline < heap->nodes[min]->deadline)
			min = son1;
		if (son2 < heap->size
			&& heap->nodes[son2]->deadline < heap->nodes[min]->deadline)
			min = son2;
		if (min == index)
			break ;
		ft_swap(heap->nodes[index], heap->nodes[min]);
		index = min;
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
	if (coder->times == 0)
		heap->nodes[size]->deadline = coder->sim->params->time_burnout;
	else
		heap->nodes[size]->deadline = coder->last_compile
			+ coder->sim->params->time_burnout;
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
