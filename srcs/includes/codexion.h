/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 15:32:33 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/15 12:15:44 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_sim	t_sim;

// ENUMERAIONS AND STRUCTURES

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef enum e_coder_status
{
	IDLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_coder_status;

typedef enum e_log_op
{
	LOG_GRAB,
	LOG_COMPILE,
	LOG_DEBUG,
	LOG_REFACTOR,
	LOG_BURNOUT
}	t_log_op;

typedef struct s_params
{
	int			num;
	long		time_burnout;
	long		time_compile;
	long		time_debug;
	long		time_refactor;
	int			required;
	int			cooldown;
	t_scheduler	scheduler;
}	t_params;

typedef struct s_dongle
{
	int				id;
	int				owned;
	long			cool;
	pthread_mutex_t	*mutex;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	t_coder_status	status;
	long			last_compile;
	int				times;
	int				permission;
	t_dongle		*left;
	t_dongle		*right;
	pthread_mutex_t	*mutex;
	pthread_mutex_t	*go;
	pthread_cond_t	*wait;
	t_sim			*sim;
}	t_coder;

typedef struct s_heap_node
{
	t_coder	*coder;
	long	deadline;
	int		times;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node		**nodes;
	size_t			size;
	size_t			capacity;
	t_scheduler		mode;
	pthread_mutex_t	*mutex;
	t_sim			*sim;
}	t_heap;

typedef struct s_request
{
	t_coder	*coder;
	size_t	request_seq;
}	t_request;

typedef struct s_log_node
{
	t_coder				*coder;
	long				time;
	t_log_op			op;
	struct s_log_node	*next;
}	t_log_node;

typedef struct s_logger
{
	pthread_mutex_t	*mutex;
	pthread_cond_t	*wait;
	t_log_node		*logs;
	t_log_node		*last;
	t_sim			*sim;
	int				silence;
}	t_logger;

typedef struct s_sim
{
	t_params		*params;
	t_coder			**coders;
	t_dongle		**dongles;
	long			start_time;
	long			last_cool;
	int				started;
	int				stop;
	int				finished;
	t_logger		*logger;
	t_heap			*heap;
	size_t			request_seq;
	pthread_mutex_t	*mutex;
	pthread_cond_t	*start;
	pthread_cond_t	*request_wait;
}	t_sim;

// FUNCTIONS

// Parsing functions

int			parse_args(int argc, char **argv);
int			ft_ispos(char *s);
int			ft_isdigit(char c);
long		ft_atol(const char *arg);
t_params	*parse_params(char **argv);

// Init functions

t_coder		**create_coders(t_sim *sim);
t_coder		*init_coder(size_t i);
t_dongle	**create_dongles(t_params *params);
t_dongle	*init_dongle(size_t i);
t_heap		*create_heap(t_sim *sim);
int			init_nodes(t_heap *heap);
t_logger	*create_logger(t_sim *sim);

// Free functions

void		free_sim(t_sim *sim);
void		free_coder(t_coder *coders);
void		free_coders(t_coder **coders, int n);
void		free_dongles(t_dongle **dongles, int n);
void		free_heap_node(t_heap_node *node);
void		free_heap(t_heap *heap);
void		free_log(t_log_node *log);
void		free_logger(t_logger *logger);

// Simulation

void		preparatives(t_sim *sim);
pthread_t	*create_threads(t_sim *sim);
int			sim_should_stop(t_sim *sim);
void		director_routine(t_sim *sim);
void		attend_request(t_sim *sim);
void		grant_permission(t_sim *sim, t_coder *coder);
void		tell_to_stop(t_sim *sim);
//void		fifo(t_sim *sim, t_heap_node *first);
//void		edf(t_sim *sim, t_heap_node *first);

// Coders

void		*coder_routine(void *arg);
void		send_request(t_coder *coder);
int			compile(t_coder *coder, t_dongle **dongles);
void		debug(t_coder *coder);
void		refactor(t_coder *coder);
void		burnout(t_coder *coder);

// Dongles

void		lock_dongles(t_dongle **dongles, int i, int n);
void		grab_dongles(t_coder *coder, t_dongle **dongles);
void		release_dongles(t_coder *coder);
void		cool_dongles(t_sim *sim);
void		check_dongles(t_dongle **dongles, int i, int n);

// Heap

void		ft_swap(t_heap_node *a, t_heap_node *b);
void		enqueue(t_heap *heap, t_coder *coder);
void		dequeue(t_heap *heap);
void		heap_up(t_heap *heap, size_t index);
void		heap_down(t_heap *heap, size_t index);
size_t		heap_size(t_heap *heap);
//void		next_request(t_heap *heap);
//void		tie_break(t_heap *heap);
//void		even(t_heap *heap);
//void		odd(t_heap *heap);
//void		tiebreaker(t_heap *heap);

// Logger

void		*logger_routine(void *arg);
void		send_log(t_coder *coder, t_log_op op, long timestamp);
void		enqueue_log(t_logger *logger, t_log_node *new);
//t_log_node	*log_last(t_logger *logger);
int			print_log(t_logger *logger);

// Other

void		ft_sleep(long time);
long		now(void);
int			wait(t_coder *coder);

#endif
