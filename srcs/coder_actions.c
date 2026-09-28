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
	//printf("REQUEST coder %d\n", coder->id);
	if (enqueue(coder->sim->heap, coder))
		return (1);
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
		/*printf("MONITOR t=%ld coder=%d deadline=%ld times=%d\n",
			t,
			coders[i]->id,
			coders[i]->deadline,
			coders[i]->times);*/
		if (coders[i]->times < coders[i]->sim->required)
		{
			done = 0;
			if (t >= coders[i]->deadline)
			{
				pthread_mutex_unlock(&coders[i]->mutex);
				return (send_log_at(coders[i], "burnout", t), i + 1);
			}
		}
		pthread_mutex_unlock(&coders[i]->mutex);
		i++;
	}
	return (done);
}

int	wait(t_coder *coder)
{
	pthread_mutex_lock(&coder->go);
	while (!coder->permission && !sim_should_stop(coder->sim))
		pthread_cond_wait(&coder->cond, &coder->go);
	if (coder->permission)
	{
		coder->permission = 0;
		return (pthread_mutex_unlock(&coder->go), 1);
	}
	return (pthread_mutex_unlock(&coder->go), 0);
}

/*int compile(t_coder *coder)
{
	int done;

	if (!wait(coder))
		return (1);

	pthread_mutex_lock(&coder->mutex);

	coder->deadline = now() + coder->sim->time_burnout;
	coder->times++;
	done = coder->times >= coder->sim->required;

	pthread_mutex_unlock(&coder->mutex);

	send_log(coder, "compile");

	usleep(coder->sim->time_compile);

	release_dongles(coder);

	return done;
}*/

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
	//printf("CODER %d entering compile\n", coder->id);
	grab_dongles(coder, coder->sim->dongles);
	if (coder->left && coder->right)
	{
		send_log(coder, "compile");
		usleep(coder->sim->time_compile);
		pthread_mutex_lock(&coder->mutex);
		coder->times++;
		coder->deadline = now() + coder->sim->time_burnout;
		done = (coder->times >= coder->sim->required);
		pthread_mutex_unlock(&coder->mutex);
		release_dongles(coder);
	}
	//printf("CODER %d leaving compile\n", coder->id);
	return (done);
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
