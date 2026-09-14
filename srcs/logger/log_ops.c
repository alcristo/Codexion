/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:00:40 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 12:41:58 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

t_log_node	*log_last(t_logger *logger) {
	t_log_node	*current;

	if (!logger->logs)
		return (NULL);
	current = logger->logs;
	while (current->next)
		current = current->next;
	return (current);
}

void	print_log(t_logger) {
	t_log_node	*first;

	pthread_mutex_lock(logger->writing)
	first = logger->logs;
	logger->logs = logger->logs->next;
	if (!strcmp(first->op, "burnout"))
	{
		printf("%ld %d burned out\n", first->time, first->coder->id);
		pthread_mutex_unlock(logger->writing);
		pthread_mutex_lock(logger->writing);
		free_logger(logger);
		pthread_mutex_unlock(logger->writing);
		return ;
	}
	else if (!strcmp(first->op, "grab"))
		printf("%ld %d is grabbing a dongle\n", first->time, first->coder->id);
	else if (!strcmp(first->op, "compile"))
		printf("%ld %d is compiling\n", first->time, first->coder->id);
	else if (!strcmp(first->op, "debug"))
		printf("%ld %d is debugging\n", first->time, first->coder->id);
	else if (!strcmp(first->op, "refactor"))
		printf("%ld %d is refactoring\n", first->time, first->coder->id);
	pthread_mutex_unlock(logger->writing);
	free_log(first);
}

void	enqueue_log(t_logger *logger, t_log_node *new) {
	if (!new)
		return ;
	if (!logger->logs)
	{
		logger->logs = new;
		return ;
	}
	log_last(logger->logs)->next = new;
}

void	*send_log(t_coder *coder, char *op, long timestamp) {
	t_log_node	*new;

	new = malloc(sizeof(t_log_node));
	if (!new)
		return ;
	new->coder = coder;
	new->msg = op;
	new->time = timestamp;
	enqueue_log(logger, new);
}
