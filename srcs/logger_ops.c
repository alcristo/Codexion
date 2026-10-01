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

void	write_message(t_coder *coder, char *action)
{
	static int	i = 0;
	int		color[2];
	char	*sentence;

	memset(&sentence, 0, sizeof(char *));
	pthread_mutex_lock(&coder->sim->logger->mutex);
	if (!strcmp("take", action))
		sentence = "has taken a dongle";
	else if (!strcmp("compile", action))
		sentence = "is compiling";
	else if (!strcmp("debug", action))
		sentence = "is debugging";
	else if (!strcmp("refactor", action))
		sentence = "is refactoring";
	else if (!strcmp("burnout", action))
		sentence = "burned out";
	else
		return ;
	color[0] = coder->id % 8 + 8;
	color[1] = (coder->id + 4) % 8 + 8;
	i++;
	printf("i = %d | %ld \x1b[38;5;%dm%d\x1b[39m %s\n",
		i, (now() - coder->sim->start_time) / 1000, color[0], coder->id, sentence);
	pthread_mutex_unlock(&coder->sim->logger->mutex);
}

/*static void	write_message(t_log *log, int i)
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
	printf("i=%d | %ld \x1b[38;5;%dm%d\x1b[39m %s\n",
		i, log->timestamp, color[0], log->id, action);
}*/

void	print_log(t_logger *logger)
{
	t_log	*logs;
	t_log	*next;
	static int		i = 0;

	pthread_mutex_lock(&logger->mutex);
	logs = logger->logs;
	logger->logs = NULL;
	pthread_mutex_unlock(&logger->mutex);
	while (logs)
	{
		next = logs->next;
		if (!logger->silence)
		{
			//write_message(logs, i);
			i++;
			if (!strcmp(logs->action, "burnout"))
				logger->silence++;
		}
		free(logs);
		logs = next;
	}
	/*while (logs)
	{
		next = logs->next;
		free(logs);
		logs = next;
	}*/
}

void	enqueue_log(t_logger *logger, t_log *log)
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
}
