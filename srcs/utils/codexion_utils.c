/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:10:34 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 12:10:12 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

long	now(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (-1);
	return ((long)tv.tv_sec * 1000L + tv.tv_usec / 1000L);
}

void	ft_sleep(long time)
{
	long	start_time;

	start_time = now();
	while (now() - start_time < time)
		usleep(10);
}
