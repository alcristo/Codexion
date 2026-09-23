/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:56:50 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 16:01:46 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	cool_dongles(t_sim *sim)
{
	int			i;
	int			n;
	long		dt;
	t_dongle	**dongles;

	dongles = sim->dongles;
	dt = now() - sim->last_cool;
	i = 0;
	n = sim->params->num;
	while (i < n)
	{
		pthread_mutex_lock(dongles[i]->mutex);
		if (sim->dongles[i]->owned)
		{
			pthread_mutex_unlock(dongles[i]->mutex);
			i++;
			continue ;
		}
		dongles[i]->cool -= dt;
		if (dongles[i]->cool < 0)
			dongles[i]->cool = 0;
		pthread_mutex_unlock(dongles[i]->mutex);
		i++;
	}
	sim->last_cool += dt;
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;

	pthread_mutex_lock(coder->mutex);
	left = coder->left;
	right = coder->right;
	if (left->id < right->id)
	{
		pthread_mutex_lock(left->mutex);
		pthread_mutex_lock(right->mutex);
	}
	else
	{
		pthread_mutex_lock(right->mutex);
		pthread_mutex_lock(left->mutex);
	}
	left->owned = 0;
	coder->left = NULL;
	pthread_mutex_unlock(left->mutex);
	right->owned = 0;
	coder->right = NULL;
	pthread_mutex_unlock(right->mutex);
	pthread_mutex_unlock(coder->mutex);
	if (!left || !right)
		return ;
}

void	grab_dongles(t_coder *coder, t_dongle **dongles)
{
	int		i;
	int		n;
	long	t;

	i = coder->id - 1;
	n = coder->sim->params->num;
	coder->left = dongles[i % n];
	t = now() - coder->sim->start_time;
	send_log(coder, LOG_GRAB, t);
	coder->right = dongles[(i + 1) % n];
	t = now() - coder->sim->start_time;
	send_log(coder, LOG_GRAB, t);
}
