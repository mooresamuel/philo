/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:15:12 by samoore           #+#    #+#             */
/*   Updated: 2024/11/27 14:05:21 by samoore          ###   ########.fr       */
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
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define SEM_FORK "/sem_fork"
#define SEM_PRINT "/sem_print"
#define SEM_END "/sem_end"
#define SEM_DEAD "/sem_dead"

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
	sem_t			*forks;
	sem_t			*print_lock;
	sem_t			*end_lock;
	atomic_int		end;
	atomic_int		num_philos;
	atomic_int		philo;
	atomic_int		wait;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	atomic_int		times_to_eat;
	long			start_time;
	atomic_int		has_first_fork;
	atomic_int		has_second_fork;
}					t_philos;

typedef struct s_timer_data
{
	t_philos	*philo;
	atomic_int	die_time;
	atomic_int	old_eat;
}				t_timer_data;

//init.c
t_philos		*init_philos(int num_philos, int argc, char **argv);
int				my_atoi(char *str);
void			*pointer_to(t_type type);

//forks.c
// pthread_mutex_t	*get_forks(int num_philos);
int				first_fork(int num_philos, int philo);
int				second_fork(int num_philos, int philo);
int				take_fork(t_philos *philo);
void			return_fork(t_philos *philo);

//time.c
long			*start_time(void);
long			get_time_since_start(void);
void			*death_timer(void *arg);
void			*end_checker(void *arg);

//print.c
int				lock_print(t_philos *philo, t_philo_state state);

//semaphore.c
void			close_sem(t_philos *philo);
void			unlink_sem(void);
