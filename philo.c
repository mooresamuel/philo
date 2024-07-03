/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/07/03 21:06:10 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	get_time_since(int start_time, int silent)
{
	struct	timeval time;
	int		elapsed_time;

	gettimeofday(&time, NULL);
	elapsed_time = (time.tv_sec * 1000 + time.tv_usec / 1000) - start_time;
	if (!silent)
		printf("%d ", elapsed_time);
	return (elapsed_time);
}

void	lock_print(t_philos *philo, e_philo_state state)
{
	pthread_mutex_lock(philo->print_lock);
	if (!*(philo->dead))
	{
		get_time_since(philo->start_time, 0);
		if (state == EATING)
			printf("philo %d is eating\n", philo->philo);
		else if (state == SLEEPING)
			printf("philo %d is sleeping\n", philo->philo);
		else if (state == THINKING)
			printf("philo %d is thinking\n", philo->philo);
		else if (state == TAKEN_FORK)
			printf("philo %d has taken a fork\n", philo->philo); 
		else
			printf("philo %d has died\n", philo->philo);
	}
	pthread_mutex_unlock(philo->print_lock);
}

void	*start_timer(void *arg)
{
	t_philos	**philo;
	int			time_left;

	philo = arg;
	pthread_mutex_lock(&(*philo)->struct_lock);
	time_left = (*philo)->time_left * 1000;
	pthread_mutex_unlock(&(*philo)->struct_lock);	
	usleep(time_left);
	pthread_mutex_lock(&(*philo)->struct_lock);
	if ((*philo)->eaten)
	{
		pthread_mutex_unlock(&(*philo)->struct_lock);	
		return (NULL);
	}
	lock_print(*philo, DIED);
	*((*philo)->dead) = 1;
	pthread_mutex_unlock(&(*philo)->struct_lock);	
	return (NULL);
}

void *new_philosopher(void *arg) 
{
	t_philos *philo;
	pthread_t timer;
	int time_since_last = 0;

	philo = arg;
	if (philo->philo % 2 == 1)
		usleep(1000);
	while (!*(philo->dead))
	{
		timer = 0;
		pthread_mutex_lock(&philo->struct_lock);
		philo->eaten = 0;
		time_since_last = get_time_since(philo->last_meal, 1);
		philo->time_left = (philo->die_time);
		pthread_create(&timer, NULL, &start_timer, (void*)&philo);
		pthread_mutex_unlock(&philo->struct_lock);	
		// lock_print(philo, THINKING);
		pthread_mutex_lock(&philo->forks[philo->first_fork]);
		lock_print(philo, TAKEN_FORK);
		pthread_mutex_lock(&philo->forks[philo->second_fork]);
		pthread_mutex_lock(&philo->struct_lock);
		philo->eaten = 1;
		pthread_mutex_unlock(&philo->struct_lock);
		lock_print(philo, EATING);
		usleep(philo->eat_time * 1000);
		pthread_mutex_lock(&philo->struct_lock);
		philo->last_meal = get_time_since(philo->start_time, 1);
		pthread_mutex_unlock(&philo->struct_lock);
		pthread_mutex_unlock(&philo->forks[philo->first_fork]);
		pthread_mutex_unlock(&philo->forks[philo->second_fork]); 
		lock_print(philo, SLEEPING);
		usleep(philo->sleep_time * 1000);
		pthread_join(timer, NULL);
	}
    return NULL; 
} 

int main(void) 
{ 
    int i = -1; 
    int error;
	int num_philos = 2;
	t_philos *philos;
	pthread_mutex_t	print_lock;
	pthread_t tid[num_philos];
	atomic_int dead = 0;

	if (pthread_mutex_init(&print_lock, NULL) != 0) { 
		printf("\n mutex init has failed\n"); 
		return 1; 
	}
	philos = init_philos(num_philos, &print_lock, &dead);
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