/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 10:52:17 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/13 11:47:22 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*void	take_dongles(t_coder *coder, t_dongle **dongles) {
	size_t	i;

	i = coder->id;
	if (i % 2 == 0)
	{
		pthread_mutex_lock(dongles[i - 1 % coders->sim->params->num]->mutex);
		pthread_mutex_lock(dongles[i % coders->sim->params->num]->mutex);
	}
	else
	{
		pthread_mutex_lock(dongles[i % coders->sim->params->num]->mutex);
		pthread_mutex_lock(dongles[i - 1 % coders->sim->params->num]->mutex);
	}
}

void	compile(t_coder *coder) {
	usleep(time_to_compile * 1000);
	coder->times++;
}

void	leave_dongles(t_coder *coder, t_dongle **dongles) {
	size_t	i;

	i = coder->id;
	pthread_mutex_unlock(dongles[i - 1 % coders->sim->params->num]->mutex);
	pthread_broadcast();
	pthread_mutex_unlock(dongles[i % coders->sim->params->num]->mutex);
	pthread_broadcast();
}*/

/*void take_dongle(t_coder *coder, t_dongle *dongle) {
	size_t	n;

	n = coder->sim->params->num;
	if (coder->id == dongle->id && !coder->left)
	{
		pthread_mutex_lock(dongle);
		coder->left = dongle;
	}
	else if ((coder->id + 1) % n == dongle->id % n && !coder->right)
	{
		pthread_mutex_lock(dongle);
		coder->right = dongle;
	}
	else
		return ;
}*/

int	compile(t_coder *coder, t_dongle **dongles) {
	size_t	i;
	int		n;

	i = coder->id;
	n = coder->sim->params->num;
	pthread_cond_timedwait(wait, go, coder->sim->params->time_burnout);
	pthread_mutex_lock(dongles[i % n]->free);
	pthread_mutex_lock(dongles[(i + 1) % n]->free);
	coder->left = dongles[i % n];
	coder->right = dongles[(i + 1) % n];
	coder->status++;
	usleep(coder->sim->params->time_compile);
	coder->times++;
	pthread_mutex_unlock(coder->left->free);
	coder->left = NULL;
	pthread_cond_broadcast(coder->sim->cond);
	pthread_mutex_unlock(coder->right->free);
	coder->right = NULL;
	pthread_cond_broadcast(coder->sim->cond);
	pthread_mutex_lock(coder->go);
	coder->status++;
	return (0);
}

void	debug(size_t i, t_coder *coder) {
	usleep(coder->sim->time_debug);
	coder->status++;
}

void	refactor(size_t i, t_coder *coder) {
	usleep(coder->sim->time_refactor);
	coder->status = IDLE;
}
