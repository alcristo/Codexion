/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:00:23 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 10:48:48 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_heap_node(t_heap_node *node)
{
	if (node)
	{
		node->coder = NULL;
		free(node);
	}
}

void	free_heap(t_heap *heap)
{
	size_t	i;

	if (!heap)
		return ;
	i = 0;
	while (i < heap->capacity)
		free_heap_node(heap->nodes[i++]);
	free(heap->nodes);
	if (heap->mutex)
	{
		pthread_mutex_destroy(heap->mutex);
		free(heap->mutex);
	}
	heap->sim = NULL;
	free(heap);
}

int	init_nodes(t_heap *heap)
{
	size_t		i;
	t_heap_node	*node;

	i = 0;
	while (i < (size_t)heap->capacity)
	{
		node = malloc(sizeof(t_heap_node));
		if (!node)
			return (1);
		node->coder = NULL;
		node->deadline = 0;
		heap->nodes[i++] = node;
	}
	return (0);
}

t_heap	*create_heap(t_sim *sim)
{
	t_heap		*heap;
	t_params	*params;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	memset(heap, 0, sizeof(t_heap));
	params = sim->params;
	heap->nodes = malloc(params->num * sizeof(t_heap_node *));
	if (!heap->nodes)
		return (free_heap(heap), NULL);
	memset(heap->nodes, 0, params->num * sizeof(t_heap_node *));
	heap->size = 0;
	heap->capacity = params->num;
	if (init_nodes(heap))
		return (free_heap(heap), NULL);
	heap->mutex = malloc(sizeof(pthread_mutex_t));
	if (!heap->mutex)
		return (free_heap(heap), NULL);
	pthread_mutex_init(heap->mutex, NULL);
	heap->mode = params->scheduler;
	heap->sim = sim;
	return (heap);
}
