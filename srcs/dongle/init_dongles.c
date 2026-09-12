/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:05:56 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 16:37:12 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_dongles(t_dongle **dongles) {
	int	i;

	i = 0;
	while (dongles[i++])
	{
		pthread_mutex_destroy(dongles[i]);
		free(dongles[i]);
	}
	free(dongles);
}

t_dongle	*init_dongle(size_t i) {
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->id = i;
	pthread_mutex_init(dongle->mutex, NULL);
	return (dongle);
}

t_dongle	**create_dongles(t_params *params) {
	t_dongle	**dongles;
	size_t		i;

	dongles = malloc(params->num * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	i = 0;
	while (i++ < params->num)
	{
		dongles[i] = init_dongle(i + 1);
		if (!dongles[i])
			return (free_dongles(dongles), NULL);
	}
	return (dongles);
}
