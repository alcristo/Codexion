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
