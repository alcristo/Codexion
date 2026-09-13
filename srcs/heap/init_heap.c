/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:00:23 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/13 14:45:25 by alcristo         ###   ########.fr       */
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
}

void	free_heap(t_heap *heap) {
	int	i;

	i = 0;
	while (heap->nodes[i++])
		free_heap_node(heap->nodes[i]);
	free(heap);
}

t_heap	*create_heap(t_params *params) {
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->nodes = malloc(params->num * sizeof(t_heap_node *));
	if (!heap->nodes)
		return (free(heap), NULL);
	heap->size = 0;
	heap->capacity = params->num;
	heap->mode = params->scheduler;
	return (heap);
}
