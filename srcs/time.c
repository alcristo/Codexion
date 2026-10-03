/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 13:04:34 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:41:29 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long	timestamp(long start)
{
	return ((now() - start) / 1000L);
}

long	now(void)
{
	struct timeval	tv;
	long			now;

	if (gettimeofday(&tv, NULL))
		return (-1);
	now = tv.tv_sec * 1000000L + tv.tv_usec;
	return (now);
}
