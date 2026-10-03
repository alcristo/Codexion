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

/*static void	write_message(t_log *log, int silence)
{
	int		color[2];
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
	else
		return ;
	color[0] = log->id % 8 + 8;
	color[1] = (log->id + 4) % 8 + 8;
	if (silence < 0)
		printf("%ld \x1b[38;5;%dm%d\x1b[39m %s\n",
			log->timestamp, color[0], log->id, action);
}*/

static char	*get_phrase(char *doing)
{
	char	*action;

	memset(&action, 0, sizeof(char *));
	if (!strcmp("take", doing))
		action = "\x1b[38;2;255;255;0mhas taken a dongle\x1b[39m";
	else if (!strcmp("compile", doing))
		action = "\x1b[38;2;0;255;0mis compiling\x1b[39m";
	else if (!strcmp("debug", doing))
		action = "\x1b[38;2;0;255;255mis debugging\x1b[39m";
	else if (!strcmp("refactor", doing))
		action = "\x1b[38;2;255;0;255mis refactoring\x1b[39m";
	else if (!strcmp("burnout", doing))
		action = "\x1b[38;2;255;0;0mburned out\x1b[39m";
	return (action);
}

void	write_message(t_coder *coder, char *doing)
{
	int		color;
	char	*action;

	pthread_mutex_lock(&coder->sim->logging);
	if (coder->sim->silence)
	{
		pthread_mutex_unlock(&coder->sim->logging);
		return ;
	}
	if (!strcmp("burnout", doing))
		coder->sim->silence++;
	//memset(&action, 0, sizeof(char *));
	action = get_phrase(doing);
	if (!action)
	{
		pthread_mutex_unlock(&coder->sim->logging);
		return ;
	}
	color = coder->id % 8 + 8;
	printf("%ld \x1b[38;5;%dm%d\x1b[39m %s\n",
		timestamp(coder->sim->start_time), color, coder->id, action);
	pthread_mutex_unlock(&coder->sim->logging);
}

/*void	print_log(t_logger *logger)
{
	t_log	*logs;
	t_log	*next;

	pthread_mutex_lock(&logger->mutex);
	logs = logger->logs;
	logger->logs = NULL;
	pthread_mutex_unlock(&logger->mutex);
	while (logs)
	{
		next = logs->next;
		write_message(logs, logger->silence);
		if (!strcmp(logs->action, "burnout"))
		{
			logger->silence++;
			return ;
		}
		free(logs);
		logs = next;
	}
	while (logs)
	{
		next = logs->next;
		free(logs);
		logs = next;
	}
}*/

/*void	enqueue_log(t_logger *logger, t_log *log)
{
	t_log	*current;

	if (!log)
		return ;
	if (!logger->logs || log->timestamp < logger->logs->timestamp)
	{
		log->next = logger->logs;
		logger->logs = log;
	}
	else
	{
		current = logger->logs;
		while (current->next && current->next->timestamp <= log->timestamp)
			current = current->next;
		log->next = current->next;
		current->next = log;
	}
	pthread_cond_broadcast(&logger->cond);
	pthread_mutex_unlock(&logger->mutex);
}

void	send_log_at(t_coder *coder, char *action, long t)
{
	t_log	*log;

	log = malloc(sizeof(t_log));
	if (!log)
		return (tell_to_stop(coder->sim));
	memset(log, 0, sizeof(t_log));
	log->id = coder->id;
	log->action = action;
	log->timestamp = (t - coder->sim->start_time) / 1000;
	pthread_mutex_lock(&coder->sim->logger->mutex);
	enqueue_log(coder->sim->logger, log);
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
	pthread_mutex_lock(&coder->sim->logger->mutex);
	enqueue_log(coder->sim->logger, log);
}*/
