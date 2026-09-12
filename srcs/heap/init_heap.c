/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_heap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 17:00:23 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 17:07:05 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

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
	return (heap);
}
