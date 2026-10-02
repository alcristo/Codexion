/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:26:14 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:25:07 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_requests(t_node *a, t_node *b)
{
	t_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	heap_before(t_node *a, t_node *b, int mode)
{
	if (!mode)
		return (a->order < b->order);
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (a->times != b->times)
		return (a->times < b->times);
	if ((a->coder->id % 2) != (b->coder->id % 2))
		return (a->coder->id % 2 == 1);
	if (a->request_id != b->request_id)
		return (a->request_id < b->request_id);
	return (a->coder->id > b->coder->id);
}
