/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 12:03:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:26:13 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongles(t_dongle **dongles)
{
	int	i;

	i = 0;
	while (dongles[i])
	{
		pthread_mutex_destroy(&dongles[i]->mutex);
		free(dongles[i]);
		i++;
	}
	free(dongles);
}

void	free_coders(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		coders[i]->sim = NULL;
		coders[i]->left = NULL;
		coders[i]->right = NULL;
		pthread_mutex_destroy(&coders[i]->mutex);
		pthread_mutex_destroy(&coders[i]->go);
		pthread_cond_destroy(&coders[i]->cond);
		free(coders[i]);
		i++;
	}
	free(coders);
}

t_dongle	**create_dongles(t_sim *sim)
{
	t_dongle	**dongles;
	int			i;

	dongles = malloc((sim->number + 1) * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	memset(dongles, 0, (sim->number + 1) * sizeof(t_dongle *));
	i = 0;
	while (i < sim->number)
	{
		dongles[i] = malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (free_dongles(dongles), NULL);
		memset(dongles[i], 0, sizeof(t_dongle));
		if (pthread_mutex_init(&dongles[i]->mutex, NULL))
			return (free_dongles(dongles), NULL);
		i++;
	}
	return (dongles);
}

static int	init_coder_pthreads(t_coder *coder)
{
	if (pthread_mutex_init(&coder->mutex, NULL))
		return (1);
	if (pthread_mutex_init(&coder->go, NULL))
	{
		pthread_mutex_destroy(&coder->mutex);
		return (1);
	}
	if (pthread_cond_init(&coder->cond, NULL))
	{
		pthread_mutex_destroy(&coder->go);
		pthread_mutex_destroy(&coder->mutex);
		return (1);
	}
	return (0);
}

t_coder	**create_coders(t_sim *sim)
{
	t_coder	**coders;
	int		i;

	coders = malloc((sim->number + 1) * sizeof(t_coder *));
	if (!coders)
		return (NULL);
	memset(coders, 0, (sim->number + 1) * sizeof(t_coder *));
	i = 0;
	while (i < sim->number)
	{
		coders[i] = malloc(sizeof(t_coder));
		if (!coders[i])
			return (free_coders(coders), NULL);
		memset(coders[i], 0, sizeof(t_coder));
		if (init_coder_pthreads(coders[i]))
		{
			free(coders[i]);
			coders[i] = NULL;
			return (free_coders(coders), NULL);
		}
		coders[i]->id = i + 1;
		coders[i]->sim = sim;
		i++;
	}
	return (coders);
}
