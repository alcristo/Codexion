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

#include "codexion.h"

long	now(void)
{
	struct timeval	tv;
	long			now;

	gettimeofday(&tv, NULL);
	now = tv.tv_sec * 1000000L + tv.tv_usec;
	return (now);
}
