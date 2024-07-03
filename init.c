/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:14:24 by samoore           #+#    #+#             */
/*   Updated: 2024/07/03 21:05:35 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

pthread_mutex_t	*init_forks(int num_philos)
{
	static pthread_mutex_t	*forks = NULL;
	int						i;

	if (!forks)
	{
		forks = malloc(sizeof(pthread_mutex_t) * num_philos);
		i = -1;
		while (++i < num_philos)
		{
			if (pthread_mutex_init(&forks[i], NULL) != 0) { 
				printf("\n mutex init has failed\n"); 
				return NULL; 
			}
		}
	}
	else if (num_philos == -1)
		free (forks);
	return (forks);
}

int	first_fork(int num_philos, int philo)
{
	if (philo < (philo + 1) % num_philos)
		return (philo);
	else
		return (0);
}

int second_fork(int num_philos, int philo)
{
	if (philo < (philo + 1) % num_philos)
		return ((philo + 1) % num_philos);
	else
		return (philo);
}

t_philos	*init_philos(int num_philos, pthread_mutex_t *print_lock, atomic_int *dead)
{
	struct timeval	time;
	int				start_time;
	int				i;
	t_philos		*philos;

	int	eat = 200;
	int sleep = 100;
	int die = 198;

	philos = malloc(sizeof(t_philos) * num_philos);
	gettimeofday(&time, NULL);
	start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	i = -1;
	while (++i < num_philos)
	{
		philos[i].forks = init_forks(num_philos);
		philos[i].print_lock = print_lock;
		pthread_mutex_init(&philos[i].struct_lock, NULL);
		philos[i].dead = dead;
		philos[i].eaten = 0;
		philos[i].philo = i;
		philos[i].num_philos = num_philos;
		philos[i].eat_time = eat;
		philos[i].sleep_time = sleep;
		philos[i].die_time = die;
		philos[i].last_meal = start_time;
		philos[i].start_time = start_time;
		philos[i].first_fork = first_fork(num_philos, i);
		philos[i].second_fork = second_fork(num_philos, i);
		philos[i].philo_pointer = &philos;
	}
	return (philos);
}
