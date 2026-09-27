/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:31:54 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:48:34 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	write_message(t_log *log)
{
	char	*action;

	memset(&action, 0, sizeof(char *));
	if (!strcmp("take", log->action))
		action = "has taken a dongle";
	else if (!strcmp("compile", log->action))
		action = "is compiling";
	else if (!strcmp("debug", log->action))
		action = "is debugging";
	else if (!strcmp("refactor", log->action))
		action = "is refactoring";
	else if (!strcmp("burnout", log->action))
		action = "burned out";
	printf("%ld %d %s\n", log->timestamp, log->id, action);
}

void	print_log(t_logger *logger)
{
	t_log	*log;

	pthread_mutex_lock(&logger->mutex);
	while (logger->logs)
	{
		log = logger->logs;
		if (!logger->silence)
		{
			write_message(logger->logs);
			if (!strcmp(logger->logs->action, "burnout"))
				logger->silence++;
		}
		logger->logs = logger->logs->next;
		free(log);
	}
	pthread_mutex_unlock(&logger->mutex);
}

void	enqueue_log(t_logger *logger, t_log *log)
{
	t_log	*current;

	if (!log)
		return ;
	pthread_mutex_lock(&logger->mutex);
	if (!logger->logs)
		logger->logs = log;
	else
	{
		current = logger->logs;
		while (current->next)
			current = current->next;
		current->next = log;
		/*if (!strcmp("burnout", log->action))
			logger->logs = log->next;
		else
		{
			current = logger->logs;
			while (current->next)
				current = current->next;
			current->next = log;
		}*/
	}
	pthread_cond_broadcast(&logger->cond);
	pthread_mutex_unlock(&logger->mutex);
}

void	send_log(t_coder *coder, char *action)
{
	t_log	*log;

	log = malloc(sizeof(t_log));
	if (!log)
		return (tell_to_stop(coder->sim));
	memset(log, 0, sizeof(t_log));
	log->id = coder->id;
	log->action = action;
	log->timestamp = (now() - coder->sim->start_time) / 1000;
	enqueue_log(coder->sim->logger, log);
}
