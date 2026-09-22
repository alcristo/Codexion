/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_logger.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:57:06 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 12:50:17 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	free_log(t_log_node *log)
{
	log->coder = NULL;
	free(log);
}

void	free_logger(t_logger *logger)
{
	t_log_node	*current;

	if (logger->mutex)
	{
		pthread_mutex_destroy(logger->mutex);
		free(logger->mutex);
	}
	if (logger->wait)
	{
		pthread_cond_destroy(logger->wait);
		free(logger->wait);
	}
	logger->last = NULL;
	current = logger->logs;
	while (current)
	{
		logger->logs = logger->logs->next;
		free_log(current);
		current = logger->logs;
	}
	logger->sim = NULL;
	free(logger);
}

t_logger	*create_logger(t_sim *sim)
{
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	if (!logger)
		return (NULL);
	memset(logger, 0, sizeof(t_logger));
	logger->logs = NULL;
	logger->mutex = malloc(sizeof(pthread_mutex_t));
	if (!logger->mutex)
		return (free_logger(logger), NULL);
	pthread_mutex_init(logger->mutex, NULL);
	logger->wait = malloc(sizeof(pthread_cond_t));
	if (!logger->wait)
		return (free_logger(logger), NULL);
	pthread_cond_init(logger->wait, NULL);
	logger->sim = sim;
	return (logger);
}
