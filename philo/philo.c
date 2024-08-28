/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 15:20:57 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	decrement_eat_times(pthread_mutex_t *lock, int *num)
{
	pthread_mutex_lock(lock);
	(*num)--;
	pthread_mutex_unlock(lock);
}

void	sleep_and_think(t_philos *philo)
{
	usleep(philo->eat_time * 1000);
	return_fork(philo, philo->first_fork);
	return_fork(philo, philo->second_fork);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, SLEEPING);
	usleep(philo->sleep_time * 1000);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, THINKING);
	usleep(500);
}

void	philo_main_loop(t_philos *philo, t_thread_data *data)
{
	pthread_t		timer;

	take_fork(philo, philo->first_fork);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, TAKEN_FORK);
	if (philo->first_fork == philo->second_fork)
	{
		return_fork(philo, philo->first_fork);
		usleep(philo->die_time * 1000);
	}
	take_fork(philo, philo->second_fork);
	decrement_eat_times(philo->struct_lock, &philo->times_to_eat);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, EATING);
	if (!dead(0, philo->dead_lock) && philo->times_to_eat)
	{
		pthread_mutex_lock(philo->struct_lock);
		data->old_times_to_eat = philo->times_to_eat;
		pthread_mutex_unlock(philo->struct_lock);
		pthread_create(&timer, NULL, &death_timer, (void *)data);
		pthread_detach(timer);
	}
	sleep_and_think(philo);
}

void	*new_philosopher(void *arg)
{
	t_thread_data	data;
	t_philos		*philo;
	pthread_t		timer;

	philo = arg;
	set_timer_data(&data, philo);
	philo->start_time = *start_time();
	pthread_create(&timer, NULL, &death_timer, (void *)&data);
	pthread_detach(timer);
	if (philo->philo % 2 == 0)
		usleep(1100);
	while (!dead(0, philo->dead_lock) && philo->times_to_eat)
		philo_main_loop(philo, &data);
	if (philo->has_first_fork)
		pthread_mutex_unlock(&philo->fork_locks[philo->first_fork]);
	if (philo->has_second_fork)
		pthread_mutex_unlock(&philo->fork_locks[philo->second_fork]);
	end(-1, philo->end_lock);
	while (end(0, philo->end_lock))
		usleep(1000);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philos	*philos;
	pthread_t	*tid;
	int			i;
	int			start;

	i = -1;
	start = 0;
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	tid = malloc(sizeof(pthread_t) * my_atoi(argv[1]));
	start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv, &start);
	i = -1;
	while (++i < my_atoi(argv[1]))
		pthread_create(&(tid[i]), NULL, &new_philosopher, (void *)&philos[i]);
	while (!dead(0, philos[0].dead_lock) && !end(0, philos[0].end_lock))
		usleep(100);
	while (--i >= 0)
		pthread_join(tid[i], NULL);
	get_struct_lock(CLEAN, 0);
	get_fork_locks(CLEAN, 0);
	free (philos);
	free (tid);
	return (0);
}
