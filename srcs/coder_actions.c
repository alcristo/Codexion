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

int	send_request(t_coder *coder, int requests)
{
	/*pthread_mutex_lock(&coder->mutex);
	coder->deadline = now() + coder->sim->time_burnout;
	pthread_mutex_unlock(&coder->mutex);*/
	if (enqueue(coder->sim->heap, coder, requests))
		return (1);
	//printf("REQUEST coder=%d deadline=%ld\n", coder->id, coder->deadline);
	return (0);
}

int	check_status(t_coder **coders)
{
	int		i;
	int		done;
	long	t;

	i = 0;
	done = 1;
	while (coders[i])
	{
		pthread_mutex_lock(&coders[i]->mutex);
		t = now();
		/*printf("MONITOR: id=%d now=%ld deadline=%ld compiling=%d times=%d\n",
			coders[i]->id,
			t,
			coders[i]->deadline,
			coders[i]->compiling,
			coders[i]->times);*/
		if (coders[i]->times < coders[i]->sim->required)
		{
			done = 0;
			if (t >= coders[i]->deadline && !coders[i]->compiling)
			{
				/*printf("STARVATION: id=%d now=%ld deadline=%ld\n",
        		    coders[i]->id, t, coders[i]->deadline);*/
				pthread_mutex_unlock(&coders[i]->mutex);
				//return (send_log_at(coders[i], "burnout", t), tell_to_stop(coders[i]->sim), -1);
				//return (send_log_at(coders[i], "burnout", t), -1);
				return (write_message(coders[i], "burnout"), -1);
			}
		}
		pthread_mutex_unlock(&coders[i]->mutex);
		i++;
	}
	return (done);
}

int	wait(t_coder *coder)
{
	struct timespec	ts;

	pthread_mutex_lock(&coder->mutex);
	ts.tv_sec = coder->deadline / 1000000L;
	ts.tv_nsec = (coder->deadline % 1000000L) * 1000L;
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_lock(&coder->go);
	while (!coder->permission && !sim_should_stop(coder->sim))
	{
		//pthread_cond_wait(&coder->cond, &coder->go);
		if (pthread_cond_timedwait(&coder->cond, &coder->go, &ts) == ETIMEDOUT)
			return (pthread_mutex_unlock(&coder->go), send_log(coder, "burnout"), tell_to_stop(coder->sim), -1);
	}
	if (coder->permission)
	{
		coder->permission = 0;
		return (pthread_mutex_unlock(&coder->go), 1);
	}
	return (pthread_mutex_unlock(&coder->go), 0);
}

int	compile(t_coder *coder)
{
	int	permission;
	int	done;

	permission = 0;
	done = 0;
	while (!sim_should_stop(coder->sim) && !permission)
		permission = wait(coder);
	if (sim_should_stop(coder->sim))
		return (1);
	pthread_mutex_lock(&coder->mutex);
	coder->deadline = now() + coder->sim->time_burnout;
	pthread_mutex_unlock(&coder->mutex);
	//printf("CODER %d entering compile\n", coder->id);
	grab_dongles(coder, coder->sim->dongles);
	if (coder->left && coder->right)
	{
		write_message(coder, "compile");
		pthread_mutex_lock(&coder->mutex);
		coder->deadline = now() + coder->sim->time_burnout;
		pthread_mutex_unlock(&coder->mutex);
		ft_sleep(coder->sim, coder->sim->time_compile);
		pthread_mutex_lock(&coder->mutex);
		coder->times++;
		coder->compiling = 0;
		done = (coder->times >= coder->sim->required);
		pthread_mutex_unlock(&coder->mutex);
		release_dongles(coder);
	}
	//printf("CODER %d leaving compile\n", coder->id);
	return (done);
}

void	program(t_coder *coder)
{
	write_message(coder, "debug");
	ft_sleep(coder->sim, coder->sim->time_debug);
	if (sim_should_stop(coder->sim))
		return ;
	write_message(coder, "refactor");
	ft_sleep(coder->sim, coder->sim->time_refactor);
}
