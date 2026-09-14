/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:46:57 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/13 16:31:32 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_coders(t_coder **coders) {
	int	i;

	i = 0;
	while (coders[i++])
	{
		pthread_mutex_destroy(coder[i]->mutex);
		pthread_mutex_destroy(coder[i]->go);
		pthread_cond_destroy(coder[i]->wait);
		coder[i]->sim = NULL;
		coder[i]->left = NULL;
		coder[i]->right = NULL;
		free(coders[i]);
	}
	free(coders);
}

t_coder	*init_coder(size_t i) {
	t_coder	*coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	coder->id = i;
	coder->status = IDLE;
	coder->times = 0;
	coder->permission = 0;
	coder->left = NULL;
	coder->right = NULL;
	pthread_mutex_init(coder->mutex, NULL);
	pthread_mutex_init(coder->go, NULL);
	pthread_cond_init(coder->cond, NULL);
	return (coder);
}

t_coder	**create_coders(t_params *params) {
	t_coder	**coders;
	size_t	i;

	coders = malloc(params->num * sizeof(coders *));
	if (!coders)
		return (NULL);
	i = 0;
	while (i++ < params->num)
	{
		coders[i] = init_coder(i + 1);
		if (!coders[i])
			return (free_coders(coders), NULL);
	}
	return (coders);
}
