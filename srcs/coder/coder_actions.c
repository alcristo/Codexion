/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 10:52:17 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 16:47:07 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder, t_dongle **dongles) {
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
}

void	debug(size_t i, t_coder *coder) {
	usleep(time_to_debug * 1000);
}

void	refactor(size_t i, t_coder *coder) {
	usleep(time_to_refactor * 1000);
}
