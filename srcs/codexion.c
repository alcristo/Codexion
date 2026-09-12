/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:51:42 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/12 15:46:43 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/codexion.h"

void	free_sim(t_sim *sim) {
	if (sim->params)
		free(sim->params);
	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
		free(sim->dongles);
	if (sim->logger)
		free(sim->logger);
	if (sim->heap)
		free(sim->heap);
	free(sim);
}

int main(int argc, char **argv) {
	if (parse_args(argc, argv))
		return (0);
	sim = malloc(sizeof(t_sim));
	if (!sim)
		return (0);
	sim->params = parse_params(argv);
	if (!sim->params)
		return (free_sim(sim), 0);
	sim->coders = create_coders(sim->params);
	if (!sim->coders)
		return (free_sim(sim), 0);
	sim->dongles = create_dongles(sim->params);
	if (!sim->dongles)
		return (free_sim(sim), 0);
	sim->logger = create_logger(sim->params);
	if (!sim->logger)
		return (free_sim(sim), 0);
	sim->heap = create_heap(sim->params);
	if (!sim->heap)
		return (free_sim(sim), 0);
	while (i++ < args[0])
		pthread_join(&coders[i]);
	return (0);
}
