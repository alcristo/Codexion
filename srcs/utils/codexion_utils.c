/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:10:34 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 15:46:17 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

void	sleep(long time) {
	long	t;
	long	start_time;

	t = 0;
	start_time = gettimeofday();
	while (t <= time)
	{
		t = gettimeofday() - start_time;
		usleep(10);
	}
}
