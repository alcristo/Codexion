/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:46:57 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 10:25:12 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_coder(t_coder *coder)
{
	if (!coder)
		return ;
	if (coder->mutex)
	{
		pthread_mutex_destroy(coder->mutex);
		free(coder->mutex);
	}
	if (coder->go)
	{
		pthread_mutex_destroy(coder->go);
		free(coder->go);
	}
	if (coder->wait)
	{
		pthread_cond_destroy(coder->wait);
		free(coder->wait);
	}
	coder->left = NULL;
	coder->right = NULL;
	coder->sim = NULL;
	free(coder);
}

void	free_coders(t_coder **coders, int n)
{
	int	i;

	if (!coders)
		return ;
	i = 0;
	while (i < n)
		free_coder(coders[i++]);
	free(coders);
}

static void	init_null(t_coder *coder)
{
	coder->status = IDLE;
	coder->times = 0;
	coder->permission = 0;
	coder->last_compile = 0;
	coder->left = NULL;
	coder->right = NULL;
	coder->mutex = NULL;
	coder->go = NULL;
	coder->wait = NULL;
	coder->sim = NULL;
}

t_coder	*init_coder(size_t i)
{
	t_coder	*coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	memset(coder, 0, sizeof(t_coder));
	coder->id = i;
	init_null(coder);
	coder->mutex = malloc(sizeof(pthread_mutex_t));
	coder->go = malloc(sizeof(pthread_mutex_t));
	if (!coder->mutex || !coder->go)
		return (free_coder(coder), NULL);
	coder->wait = malloc(sizeof(pthread_cond_t));
	if (!coder->wait)
		return (free_coder(coder), NULL);
	pthread_mutex_init(coder->mutex, NULL);
	pthread_mutex_init(coder->go, NULL);
	pthread_cond_init(coder->wait, NULL);
	return (coder);
}

t_coder	**create_coders(t_sim *sim)
{
	t_coder		**coders;
	size_t		i;

	coders = malloc(sim->params->num * sizeof(t_coder *));
	if (!coders)
		return (NULL);
	memset(coders, 0, sim->params->num * sizeof(t_coder *));
	i = 0;
	while (i < (size_t)sim->params->num)
	{
		coders[i] = init_coder(i + 1);
		if (!coders[i])
			return (free_coders(coders, i), NULL);
		coders[i]->sim = sim;
		i++;
	}
	return (coders);
}
