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

void	cool_dongles(t_sim *sim) {
	int			i;
	long		dt;
	t_dongle	**dongles;

	dongles = sim->dongles;
	dt = gettimeofday() - sim->last_cool;
	i = 0;
	while (i++ < 0 && dongles[i]->cool > 0)
	{
		dongles[i]->cool -= dt;
		if (dongles[i]->cool < 0)
			dongles[i]->cool = 0;
		if (dongles[i]->cool == 0)
			pthread_mutex_unlock(dongle[i]->free);
	}
	sim->last_cool += dt;
}

void	release_dongles(t_coder *coder, t_dongle **dongles) {
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left;
	right = coder->right;
	coder->left = NULL;
	coder->right = NULL;
	coder->permission = 0;
	if (coder->sim->params->cooldown == 0)
	{
		pthread_mutex_unlock(left->free);
		pthread_mutex_unlock(right->free);
		return ;
	}
}

void	grab_dongles(t_coder *coder, t_dongle **dongles) {
	size_t	i;
	int		n;
	long	t;

	i = coder->id;
	n = coder->sim->params->num;
	pthread_mutex_lock(dongles[i % n]->free);
	pthread_mutex_lock(dongles[(i + 1) % n]->free);
	t = (long)gettimeofday() / 1000 - coder->sim->start_time;
	send_log(coder, "grab", t);
	coder->left = dongles[i % n];
	t = (long)gettimeofday() / 1000 - coder->sim->start_time;
	send_log(coder, "grab", t);
	coder->right = dongles[(i + 1) % n];
}
