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

#include "codexion.h"

/*void	free_node(t_node *node)
{
	node->coder = NULL;
	free(node);
}*/

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

/*static void	init_nodes(t_heap *heap, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		heap->nodes[i] = malloc(sizeof(t_node));
		if (!heap->nodes[i])
			return (free_nodes(heap, i), free_heap(heap));
		memset(heap->nodes[i], 0, sizeof(t_node));
		i++;
	}
}*/

t_heap	*create_heap(t_sim *sim)
{
	t_heap	*heap;
	int		i;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	memset(heap, 0, sizeof(t_heap));
	if (pthread_mutex_init(&heap->mutex, NULL))
		return (free_heap(heap), NULL);
	if (pthread_cond_init(&heap->cond, NULL))
		return (free_heap(heap), NULL);
	heap->capacity = sim->number;
	heap->mode = sim->scheduler;
	heap->sim = sim;
	heap->nodes = malloc(sim->number * sizeof(t_node *));
	if (!heap->nodes)
		return (free_heap(heap), NULL);
	memset(heap->nodes, 0, sim->number * sizeof(t_node *));
	i = 0;
	while (i < sim->number)
	{
		heap->nodes[i] = malloc(sizeof(t_node));
		if (!heap->nodes[i])
			return (free_nodes(heap, i), free_heap(heap), NULL);
		memset(heap->nodes[i], 0, sizeof(t_node));
		i++;
	}
	return (heap);
}
