/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_logger.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:57:06 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 16:59:32 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

t_logger	*create_logger() {
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	if (!logger)
		return NULL;
	pthread_mutex_init(logger->writing, NULL);
	return (logger);
}
