/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/07/02 20:35:47 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <stdatomic.h>
#include <sys/time.h>

typedef struct s_philos{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*print_lock;
	atomic_int		*dead;
	int				philo;
	int				num_philos;
	int				eat_time;
	int				think_time;
	int				sleep_time;
	int				times_to_eat;
	int				last_meal;
	int				start_time;
}					t_philos;

int	print_time(int start_time)
{
	struct	timeval time;
	int		elapsed_time;

	gettimeofday(&time, NULL);
	elapsed_time = time.tv_sec * 1000 + time.tv_usec / 1000 - start_time;
	printf("%d ", elapsed_time);
	return (elapsed_time);
}

void	lock_print(t_philos *philo, char *action, int alt)
{
	pthread_mutex_lock(philo->print_lock);
	if (!*(philo->dead))
	{
		print_time(philo->start_time);
		if (!alt)
			printf("philo %d is %s\n", philo->philo, action); 
		else if (alt == 1)
			printf("philo %d has taken a fork\n", philo->philo); 
		else
			printf("philo %d has died\n", philo->philo);
	}
	pthread_mutex_unlock(philo->print_lock);
}

void *new_philosopher(void *arg) 
{
	t_philos *philo;
	int	first_fork;
	int	second_fork;
	int count = 0;

	philo = arg;
	if (philo->philo < (philo->philo + 1) % philo->num_philos)
	{
		first_fork = philo->philo;
		second_fork = (philo->philo + 1) % philo->num_philos;
	}
	else
	{
		first_fork = 0;
		second_fork = philo->philo;
	}

	while (!*(philo->dead))
	{
		lock_print(philo, "thinking", 0);
		pthread_mutex_lock(&philo->forks[first_fork]);
		lock_print(philo, "", 1);
		pthread_mutex_lock(&philo->forks[second_fork]); 		
		lock_print(philo, "eating", 0);
		if (count == 6 && philo->philo == 3)
		{
			pthread_mutex_unlock(&philo->forks[first_fork]);
			pthread_mutex_unlock(&philo->forks[second_fork]);
			lock_print(philo, "", 2);
			*philo->dead = 1;
			return NULL;
		}	
		usleep(philo->eat_time);
		pthread_mutex_unlock(&philo->forks[first_fork]);
		pthread_mutex_unlock(&philo->forks[second_fork]); 
		usleep(philo->think_time);
		lock_print(philo, "sleeping", 0);
		usleep(philo->sleep_time);
		count++;
	}
    return NULL; 
} 

pthread_mutex_t	*init_forks(int num_philos)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	i = -1;
	while (++i < num_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0) { 
			printf("\n mutex init has failed\n"); 
			return NULL; 
		}
	}
	return (forks);
}

t_philos	*init_philos(int num_philos, pthread_mutex_t *forks, pthread_mutex_t *print_lock, atomic_int *dead)
{
	struct timeval time;
	int				start_time;
	int				i;
	t_philos		*philos;

	int	eat = 410000;
	int think = 200000;
	int sleep = 200000;

	philos = malloc(sizeof(t_philos) * num_philos);
	gettimeofday(&time, NULL);
	start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	i = 0;
	while (i < num_philos)
	{
		philos[i].forks = forks;
		philos[i].num_philos = num_philos;
		philos[i].philo = i;
		philos[i].eat_time = eat;
		philos[i].sleep_time = sleep;
		philos[i].think_time = think;
		philos[i].print_lock = print_lock;
		philos[i].dead = dead;
		philos[i].start_time = start_time;
		philos[i].last_meal = 0;
		i++;
	}
	return (philos);
}

int main(void) 
{ 
    int i = -1; 
    int error;
	int num_philos = 7;
	t_philos *philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_t tid[num_philos];
	atomic_int dead = 0;

	forks = init_forks(num_philos);
	if (pthread_mutex_init(&print_lock, NULL) != 0) { 
		printf("\n mutex init has failed\n"); 
		return 1; 
	}
	philos = init_philos(num_philos, forks, &print_lock, &dead);
	i = 0;
    while (i < num_philos) 
	{
        error = pthread_create(&(tid[i]), NULL, &new_philosopher, (void*)&philos[i]); 
        if (error != 0) 
			usleep(2);
        i++; 
    }
	i = 0;
	while (i < num_philos)
		pthread_join(tid[i++], NULL);
    return 0; 
} 