/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 16:28:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/13 13:28:12 by alcristo         ###   ########.fr       */
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
	int min;

	son1 = 2 * index + 1;
	son2 = 2 * index + 2;
	min = index;
	if (son1 < heap->size &&
			heap->nodes[son1]->deadline < heap->nodes[index]->deadline)
		min = son1;
	if (son2 < heap->size &&
			heap->nodes[son2]->deadline < heap->nodes[index]->deadline)
	{
		if heap->nodes[son2]->deadline < heap->nodes[son1]->deadline
			min = son2;
	}
	if (min == index)
		return ;
	ft_swap(&heap->nodes[index], &heap->nodes[min]);
	heap_down(heap, min);
}

void enqueue(t_heap *heap, int value) {
	if (heap->size == heap->capacity)
		return ;
	heap->nodes[heap->size] = value;
	heap->size++;
	if (heap->mode = EDF)
		heap_up(heap, heap->size - 1);
}

int	dequeue(t_heap *heap) {
	int max;

	if heap->size == 0:
		return (-1);
	max = heap->nodes[0]
	heap->size--;
	heap->nodes[0] = heap->nodes[heap->size];
	if (heap->size > 0 && heap->mode = EDF)
		heap_down(heap, 0);
	return (max)
}
