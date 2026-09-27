/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_logger.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 13:14:58 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/25 13:31:11 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_logs(t_logger *logger)
{
	t_log	*log;

	pthread_mutex_lock(&logger->mutex);
	log = logger->logs;
	while (log)
	{
		logger->logs = log->next;
		free(log);
		log = logger->logs;
	}
	pthread_mutex_unlock(&logger->mutex);
}

void	free_logger(t_logger *logger)
{
	clean_logs(logger);
	logger->sim = NULL;
	pthread_mutex_destroy(&logger->mutex);
	pthread_cond_destroy(&logger->cond);
	free(logger);
}

t_logger	*create_logger(t_sim *sim)
{
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	if (!logger)
		return (NULL);
	memset(logger, 0, sizeof(t_logger));
	if (pthread_mutex_init(&logger->mutex, NULL))
		return (free_logger(logger), NULL);
	if (pthread_cond_init(&logger->cond, NULL))
		return (free_logger(logger), NULL);
	logger->sim = sim;
	return (logger);
}
