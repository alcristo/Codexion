/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 16:28:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 17:13:29 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void ft_swap(int *a, int *b) {
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void heap_up(t_heap *heap, size_t index) {
	size_t parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (heap->nodes[parent]->deadline >= heap->nodes[index]->deadline)
			break ;
		ft_swap(heap->nodes[parent], heap->nodes[index]);
		index = parent;
	}
}

void heap_down(t_heap *heap, size_t index) {
	int	son1;
	int son2;
	int max;

	son1 = 2 * index + 1;
	son2 = 2 * index + 2;
	max = index;
	if (son1 < heap->size &&
			heap->nodes[son1]->deadline > heap->nodes[index]->deadline)
		max = son1;
	if (son2 < heap->size &&
			heap->nodes[son2]->deadline > heap->nodes[index]->deadline)
		max = son2;
	if (max == index)
		return ;
	ft_swap(&heap->nodes[index], &heap->nodes[largest]);
	heap_down(heap, largest);
}

void enqueue(t_heap *heap, int value) {
	if heap->size ==	heap->capacity
		return ;
	heap->nodes[heap->size] = value;
	heap->size++;
	heap_up(heap, heap->size - 1);
}

int	dequeue(t_heap *heap) {
	int max;

	if heap->size == 0:
		return (-1);
	max = heap->nodes[0]
	heap->size--;
	heap->nodes[0] = heap->nodes[heap->size];
	if (heap->size > 0)
		heap_down(heap, 0);
	return (max)
}
