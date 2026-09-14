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

void	free_log(t_log_node *log) {
	free_coder(log->coder);
	free(log);
}

void	free_logger(t_logger *logger) {
	t_log_node	*current;

	pthread_mutex_destroy(logger->writing);
	if (!logger->logs)
		return ;
	current = logger->logs;
	while (current)
	{
		logger->logs = logger->logs->next;
		free_log(current);
		current = logger->logs;
	}
	free(logger);
}

t_logger	*create_logger() {
	t_logger	*logger;

	logger = malloc(sizeof(t_logger));
	if (!logger)
		return NULL;
	logger->logs = NULL;
	pthread_mutex_init(logger->writing, NULL);
	return (logger);
}
