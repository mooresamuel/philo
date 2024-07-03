/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:15:12 by samoore           #+#    #+#             */
/*   Updated: 2024/07/03 20:18:51 by samoore          ###   ########.fr       */
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
	DIED
}	e_philo_state;

typedef struct s_philos{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t struct_lock;
	atomic_int		*dead;
	int				eaten;
	int				philo;
	int				num_philos;
	int				eat_time;
	int				sleep_time;
	int				die_time;
	int				time_left;
	int				times_to_eat;
	int				last_meal;
	int				start_time;
	int				first_fork;
	int				second_fork;
	struct s_philos	**philo_pointer;
}					t_philos;


pthread_mutex_t	*init_forks(int num_philos);
t_philos		*init_philos(int num_philos, pthread_mutex_t *print_lock, atomic_int *dead);