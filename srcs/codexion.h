/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 10:35:17 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/26 14:24:12 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>
# include <errno.h>

typedef struct s_sim	t_sim;

typedef struct s_dongle
{
	int				reserved;
	long			cooldown;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	int				compiling;
	int				times;
	int				permission;
	long			deadline;
	t_sim			*sim;
	t_dongle		*left;
	t_dongle		*right;
	pthread_mutex_t	mutex;
	pthread_mutex_t	go;
	pthread_cond_t	cond;
}	t_coder;

typedef struct s_node
{
	t_coder	*coder;
	long	deadline;
	int		times;
	int		order;
	int		request_id;
}	t_node;

typedef struct s_heap
{
	int				size;
	int				capacity;
	int				mode;
	int				total_requests;
	t_node			**nodes;
	t_sim			*sim;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_heap;

typedef struct s_threads
{
	pthread_t	monitor;
	pthread_t	waiter;
	pthread_t	*coders;
}	t_threads;

typedef struct s_sim
{
	int				number;
	int				time_burnout;
	int				time_compile;
	int				time_debug;
	int				time_refactor;
	int				required;
	int				cooldown;
	int				scheduler;
	long			start_time;
	int				started; // protected by sim->mutex
	int				stop; // protected by sim->mutex
	int				silence;
	t_coder			**coders; // protected by coders[i].mutex
	t_dongle		**dongles; // protected by dongles[i].mutex
	t_heap			*heap; // protected by heap.mutex
	pthread_mutex_t	mutex;
	pthread_mutex_t	logging;
	pthread_cond_t	cond;
}	t_sim;

// Parsing
int			parse_args(int argc, char **argv);

// Create structs
t_coder		**create_coders(t_sim *sim);
t_dongle	**create_dongles(t_sim *sim);
t_heap		*create_heap(t_sim *sim);

// Free structs
void		free_coders(t_coder **coders);
void		free_dongles(t_dongle **dongles);
void		free_heap(t_heap *heap);

// Threads
void		preparatives(t_sim *sim);
int			start_threads(t_threads *threads, t_sim *sim);
void		threads_failure(t_sim *sim);
void		start(t_sim *sim);
void		join_threads(t_threads *threads, int n);

// Simulation utils
int			sim_should_stop(t_sim *sim);
void		tell_to_stop(t_sim *sim);
int			check_status(t_coder **coder);
void		attend_request(t_sim *sim);
long		now(void);
long		timestamp(long start);
void		ft_sleep(t_sim *sim, int t);

// Routines
void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);
void		*waiter_routine(void *arg);

// Coder actions
int			compile(t_coder *coder);
void		program(t_coder *coder);

// Dongle actions
void		lock_order(t_dongle **dongles, int i, int n);
void		grab_dongles(t_coder *coder, t_dongle **dongles);
void		release_dongles(t_coder *coder);

//  Heap operations
void		swap_requests(t_node *a, t_node *b);
int			heap_before(t_node *a, t_node *b, int mode);
void		heap_up(t_heap *heap, int index);
void		heap_down(t_heap *heap, int index);
int			enqueue(t_heap *heap, t_coder *coder, int requests);
void		dequeue(t_heap *heap);

//Logger operations
void		write_message(t_coder *coder, char *doing);

#endif
