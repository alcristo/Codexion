/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alcristo <alcristo@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 15:32:33 by alcristo          #+#    #+#             */
/*   Updated: 2026/09/14 15:52:32 by alcristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef enum e_scheduler {
	FIFO,
	EDF
} t_scheduler;

typedef struct s_params {
	int			num;
	int			time_burnout;
	int			time_compile;
	int			time_debug;
	int			time_refactor;
	int			required;
	int			cooldown;
	t_scheduler	scheduler;
} t_params;

typedef struct s_heap_node {
	t_coder	*coder;
	size_t	deadline;
} t_heap_node;

typedef struct s_heap {
	t_heap_node	**nodes;
	size_t		size;
	size_t		capacity;
	t_scheduler	mode;
} t_heap;

typedef struct s_log_node {
	t_coder				*coder;
	long				time;
	char				*msg;
	struct s_log_node	*next;
} t_log_node;

typedef struct s_logger {
	pthread_mutex_t	*writing;
	t_log_node		*logs;
} t_logger;

typedef struct s_dongle {
	int				id;
	long			cool;
	pthread_mutex_t	*free;
	//pthread_cond_t	*cool;
} t_dongle;

typedef enum e_coder_status {
	IDLE,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
} t_coder_status;

typedef struct s_coder {
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
} t_coder;

typedef struct s_sim {
	t_params		*params;
	t_coder			**coders;
	t_dongle		**dongles;
	long			start_time;
	long			last_cool;
	int				stop;
	int				finished;
	t_logger		*logger;
	t_heap			*heap;
	//pthread_mutex_t	*stop_mutex;
	pthread_mutex_t	*heap_mutex;
	//pthread_mutex_t	*scheduler_mutex;
	pthread_cond_t	*start;
} t_sim;

#endif
