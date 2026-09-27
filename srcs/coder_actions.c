/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 16:51:42 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:47:55 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	send_request(t_coder *coder)
{
	int		error;

	error = 0;
	if (enqueue(coder->sim->heap, coder))
		error++;
	return (error);
}

int	check_status(t_coder **coders)
{
	int	i;
	int	stop;
	int	done;

	i = 0;
	stop = 0;
	done = 1;
	while (coders[i] && !stop)
	{
		pthread_mutex_lock(&coders[i]->mutex);
		if (done && coders[i]->times < coders[i]->sim->required)
			done--;
		if (now() >= coders[i]->deadline)
			stop = coders[i]->id;
		pthread_mutex_unlock(&coders[i]->mutex);
		if (stop)
			return (send_log(coders[i], "burnout"), stop);
		i++;
	}
	return (done);
}

/*static void	burnout(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	coder->burnout = 1;
	pthread_mutex_unlock(&coder->mutex);
	send_log(coder, "burnout");
}*/

int	wait(t_coder *coder)
{
	pthread_mutex_lock(&coder->go);
	while (!coder->permission && !sim_should_stop(coder->sim))
		pthread_cond_wait(&coder->cond, &coder->go);
	if (coder->permission)
		return (coder->permission = 0, pthread_mutex_unlock(&coder->go), 1);
	return (pthread_mutex_unlock(&coder->go), 0);
}

int	compile(t_coder *coder)
{
	int	permission;

	permission = 0;
	while (!sim_should_stop(coder->sim) && !permission)
		permission = wait(coder);
	if (sim_should_stop(coder->sim))
		return (0);
	grab_dongles(coder, coder->sim->dongles);
	if (coder->left && coder->right)
	{
		pthread_mutex_lock(&coder->mutex);
		coder->deadline = now() + coder->sim->time_burnout;
		coder->times++;
		pthread_mutex_unlock(&coder->mutex);
		send_log(coder, "compile");
		usleep(coder->sim->time_compile);
		release_dongles(coder);
	}
	return (0);
}

void	program(t_coder *coder)
{
	send_log(coder, "debug");
	usleep(coder->sim->time_debug);
	if (sim_should_stop(coder->sim))
		return ;
	send_log(coder, "refactor");
	usleep(coder->sim->time_refactor);
}
