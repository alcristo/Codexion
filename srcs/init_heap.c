/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 12:21:19 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:27:18 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void	free_nodes(t_heap *heap, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		free(heap->nodes[i]);
		i++;
	}
	free(heap->nodes);
	heap->nodes = NULL;
}

void	free_heap(t_heap *heap)
{
	if (heap->nodes)
		free_nodes(heap, heap->capacity);
	heap->sim = NULL;
	pthread_mutex_destroy(&heap->mutex);
	pthread_cond_destroy(&heap->cond);
	heap->sim = NULL;
	free(heap);
}

static int	init_heap(t_heap *heap, t_sim *sim)
{
	int	i;

	if (pthread_mutex_init(&heap->mutex, NULL))
		return (free_heap(heap), 1);
	if (pthread_cond_init(&heap->cond, NULL))
		return (free_heap(heap), 1);
	heap->capacity = sim->number;
	heap->mode = sim->scheduler;
	heap->sim = sim;
	heap->nodes = malloc(sim->number * sizeof(t_node *));
	if (!heap->nodes)
		return (free_heap(heap), 1);
	memset(heap->nodes, 0, sim->number * sizeof(t_node *));
	i = 0;
	while (i < sim->number)
	{
		heap->nodes[i] = malloc(sizeof(t_node));
		if (!heap->nodes[i])
			return (free_nodes(heap, i), free_heap(heap), 1);
		memset(heap->nodes[i], 0, sizeof(t_node));
		i++;
	}
	return (0);
}

t_heap	*create_heap(t_sim *sim)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	memset(heap, 0, sizeof(t_heap));
	if (init_heap(heap, sim))
		return (NULL);
	return (heap);
}
