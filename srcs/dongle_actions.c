/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:15:53 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:51:30 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	lock_order(t_dongle **dongles, int i, int n)
{
	int	left;
	int	right;

	left = i;
	right = (i + 1) % n;
	if (left < right)
	{
		pthread_mutex_lock(&dongles[left]->mutex);
		pthread_mutex_lock(&dongles[right]->mutex);
	}
	else
	{
		pthread_mutex_lock(&dongles[right]->mutex);
		pthread_mutex_lock(&dongles[left]->mutex);
	}
}

void	grab_dongles(t_coder *coder, t_dongle **dongles)
{
	int	i;
	int	n;

	n = coder->sim->number;
	i = (n + coder->id - 1) % n;
	pthread_mutex_lock(&coder->mutex);
	coder->left = dongles[i];
	write_message(coder, "take");
	coder->right = dongles[(i + 1) % n];
	write_message(coder, "take");
	pthread_mutex_unlock(&coder->mutex);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	long		cooldown;

	left = coder->left;
	right = coder->right;
	pthread_mutex_lock(&coder->mutex);
	coder->left = NULL;
	coder->right = NULL;
	pthread_mutex_unlock(&coder->mutex);
	cooldown = now() + coder->sim->cooldown;
	pthread_mutex_lock(&left->mutex);
	left->reserved = 0;
	left->cooldown = cooldown;
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_lock(&right->mutex);
	right->reserved = 0;
	right->cooldown = cooldown;
	pthread_mutex_unlock(&right->mutex);
	pthread_mutex_lock(&coder->sim->heap->mutex);
	pthread_cond_broadcast(&coder->sim->heap->cond);
	pthread_mutex_unlock(&coder->sim->heap->mutex);
}
