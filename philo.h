/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:15:12 by samoore           #+#    #+#             */
/*   Updated: 2024/07/06 14:10:04 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <stdatomic.h>
#include <sys/time.h>

typedef enum {
	THINKING,
	EATING,
	SLEEPING,
	TAKEN_FORK,
	DIED,
	LIVE
}	e_philo_state;

typedef enum {
	DEAD_LOCK,
	PRINT_LOCK,
	END_LOCK,
	INIT
}	e_type;

typedef struct s_philos{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t *struct_lock;
	pthread_mutex_t *dead_lock;
	pthread_mutex_t *end_lock;
	pthread_mutex_t *data_lock;
	atomic_int		*ready;
	pthread_t 		tid;
	int				philo;
	int				num_philos;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	atomic_int		times_to_eat;
	long			start_time;
	int				first_fork;
	int				second_fork;
}					t_philos;

typedef struct s_thread_data{
	pthread_mutex_t	*print_lock;
	pthread_mutex_t *struct_lock;
	pthread_mutex_t *dead_lock;
	pthread_mutex_t *end_lock;
	pthread_mutex_t *data_lock;
	pthread_t 		tid;
	int				philo;
	int				die_time;
	atomic_int		*times_to_eat;
	int				old_times;
	atomic_int		kill;
	long			start_time;
}					t_thread_data;

//init.c
pthread_mutex_t	*get_struct_lock(int philo, int init);
t_philos		*init_philos(int num_philos, int argc, char **argv);
int				my_atoi(char *str);
void			*pointer_to(e_type type);

//forks.c
pthread_mutex_t	*get_forks(int num_philos);
int				first_fork(int num_philos, int philo);
int				second_fork(int num_philos, int philo);

//statics.c
int				dead(int num, pthread_mutex_t *dead_lock);
int				end(int num, pthread_mutex_t *end_lock);