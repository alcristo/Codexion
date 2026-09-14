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

void	free_heap_node(t_heap_node *node) {
	node->coder->sim = NULL;
	pthread_mutex_destroy(node->coder->go);
	pthread_mutex_destroy(node->coder->mutex);
	pthread_cond_destroy(node->coder->wait);
	node->coder->left = NULL;
	node->coder->right = NULL;
	free(node->coder);
	free(node);
}

void	free_heap(t_heap *heap) {
	int	i;

	i = 0;
	while (heap->nodes[i++])
		free_heap_node(heap->nodes[i]);
	free(heap);
}

int	init_nodes(t_heap *heap) {
	int			i;
	t_heap_node	*node;

	i = 0;
	while(i++ < heap->capacity)
	{
		node = malloc(sizeof(t_node));
		if (!node)
		{
			while (--i >= 0)
				free_heap_node(heap->nodes[i]);
			return (1);
		}
		node->coder = NULL;
		node->deadline = 0;
		heap->nodes[i] == node;
	}
	return (0);
}

t_heap	*create_heap(t_params *params) {
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(params->num * sizeof(t_heap_node *));
	if (!heap->nodes)
		return (free_heap(heap), NULL);
	heap->size = 0;
	heap->capacity = params->num;
	if (init_nodes(heap))
		return (free_heap(heap), NULL);
	heap->mode = params->scheduler;
	return (heap);
}
