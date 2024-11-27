/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:15:12 by samoore           #+#    #+#             */
/*   Updated: 2024/11/27 13:56:05 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <stdatomic.h>
#include <sys/time.h>
#include <limits.h>

typedef enum s_philo_state
{
	THINKING,
	EATING,
	SLEEPING,
	TAKEN_FORK,
	DIED
}	t_philo_state;

typedef enum s_type
{
	DEAD_LOCK,
	PRINT_LOCK,
	END_LOCK,
	TAKE,
	RETURN,
	INIT,
	CLEAN
}	t_type;

typedef struct s_philos
{
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*struct_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*end_lock;
	pthread_mutex_t	*fork_locks;
	int				*ready;
	int				*forks;
	int				philo;
	int				num_philos;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	int				times_to_eat;
	long			start_time;
	int				first_fork;
	int				second_fork;
	int				has_first_fork;
	int				has_second_fork;
}					t_philos;

typedef struct s_thread_data
{
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*struct_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*end_lock;
	pthread_mutex_t	*fork_locks;
	int				philo;
	int				die_time;
	int				*times_to_eat;
	int				old_times_to_eat;
	long			start_time;
	int				first_fork;
	int				second_fork;
	int				*has_first_fork;
	int				*has_second_fork;
}					t_thread_data;

//init.c
t_philos		*init_philos(int num_philos, int argc,
					char **argv, int *start);
int				my_atoi(char *str);
void			*pointer_to(t_type type);
int				my_atoi(char *str);

//forks.c
// pthread_mutex_t	*get_forks(int num_philos);
int				first_fork(int num_philos, int philo);
int				second_fork(int num_philos, int philo);
int				take_fork(t_philos *philo, int fork);
void			return_fork(t_philos *philo, int fork);
int				fork_state(t_philos *philo, int num, int *forks, t_type action);

//statics.c
int				dead(int num, pthread_mutex_t *dead_lock);
int				end(int num, pthread_mutex_t *end_lock);
int				*get_forks(int num);
void			add_locks(t_philos *philos, char **argv);

//locks.c
pthread_mutex_t	*get_fork_locks(t_type action, int num);
pthread_mutex_t	*get_struct_lock(t_type action, int philo);
void			lock_print(int philo, pthread_mutex_t *dead_lock,
					pthread_mutex_t *print_lock, t_philo_state state);

//time.c
long			*start_time(void);
long			get_time_since_start(int silent);
void			set_timer_data(t_thread_data *data, t_philos *philo);
void			*death_timer(void *arg);
void			decrement_eat_times(pthread_mutex_t *lock, int *num);
