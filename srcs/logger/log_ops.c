/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:00:40 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 11:51:15 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

int	print_log(t_logger *logger)
{
	t_log_node	*first;

	pthread_mutex_lock(logger->mutex);
	if (!logger->logs)
		return (pthread_mutex_unlock(logger->mutex), 0);
	first = logger->logs;
	logger->logs = first->next;
	if (!logger->logs)
		logger->last = NULL;
	pthread_mutex_unlock(logger->mutex);
	if (!logger->silence)
	{
		if (first->op == LOG_BURNOUT)
		{
			printf("%ld %d burned out\n", first->time, first->coder->id);
			logger->silence = 1;
		}
		else if (first->op == LOG_GRAB)
			printf("%ld %d is grabbing a dongle\n", first->time, first->coder->id);
		else if (first->op == LOG_COMPILE)
			printf("%ld %d is compiling\n", first->time, first->coder->id);
		else if (first->op == LOG_DEBUG)
			printf("%ld %d is debugging\n", first->time, first->coder->id);
		else if (first->op == LOG_REFACTOR)
			printf("%ld %d is refactoring\n", first->time, first->coder->id);
	}
	free_log(first);
	return (1);
}

void	enqueue_log(t_logger *logger, t_log_node *new)
{
	if (!new)
		return ;
	if (!logger->logs)
	{
		logger->logs = new;
		logger->last = new;
		return ;
	}
	logger->last->next = new;
	logger->last = new;
}

void	send_log(t_coder *coder, t_log_op op, long timestamp)
{
	t_log_node	*new;

	new = malloc(sizeof(t_log_node));
	if (!new)
	{
		pthread_mutex_lock(coder->sim->mutex);
		coder->sim->stop = 1;
		pthread_mutex_unlock(coder->sim->mutex);
		return ;
	}
	new->coder = coder;
	new->op = op;
	new->time = timestamp;
	new->next = NULL;
	pthread_mutex_lock(coder->sim->logger->mutex);
	enqueue_log(coder->sim->logger, new);
	pthread_mutex_unlock(coder->sim->logger->mutex);
}
