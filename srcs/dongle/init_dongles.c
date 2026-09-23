/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:05:56 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 15:27:05 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_dongles(t_dongle **dongles, int n)
{
	int	i;

	if (!dongles)
		return ;
	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(dongles[i]->mutex);
		free(dongles[i]->mutex);
		free(dongles[i++]);
	}
	free(dongles);
}

t_dongle	*init_dongle(size_t i)
{
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	memset(dongle, 0, sizeof(t_dongle));
	dongle->id = i;
	dongle->cool = 0;
	dongle->mutex = malloc(sizeof(pthread_mutex_t));
	if (!dongle->mutex)
		return (free(dongle), NULL);
	pthread_mutex_init(dongle->mutex, NULL);
	return (dongle);
}

t_dongle	**create_dongles(t_params *params)
{
	t_dongle	**dongles;
	int			i;

	dongles = malloc(params->num * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	memset(dongles, 0,params->num * sizeof(t_dongle *));
	i = 0;
	while (i < params->num)
	{
		dongles[i] = init_dongle(i + 1);
		if (!dongles[i])
			return (free_dongles(dongles, i), NULL);
		i++;
	}
	return (dongles);
}
