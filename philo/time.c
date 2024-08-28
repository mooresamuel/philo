/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 14:48:41 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 15:20:45 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	*start_time(void)
{
	struct timeval	time;
	static long		start_time = 0;

	if (!start_time)
	{
		gettimeofday(&time, NULL);
		start_time = ((long)time.tv_sec * 1000 + (long)time.tv_usec / 1000);
	}
	return (&start_time);
}

long	get_time_since_start(int silent)
{
	struct timeval	time;
	long			elapsed_time;

	gettimeofday(&time, NULL);
	elapsed_time = (time.tv_sec * 1000 + time.tv_usec / 1000) - *start_time ();
	if (!silent)
		printf("%4ld : ", elapsed_time);
	return (elapsed_time);
}

void	set_timer_data(t_thread_data *data, t_philos *philo)
{
	data->dead_lock = philo->dead_lock;
	data->end_lock = philo->end_lock;
	data->philo = philo->philo;
	data->print_lock = philo->print_lock;
	data->struct_lock = philo->struct_lock;
	data->times_to_eat = &philo->times_to_eat;
	data->die_time = philo->die_time;
	data->has_first_fork = &philo->has_first_fork;
	data->has_second_fork = &philo->has_second_fork;
	data->fork_locks = philo->fork_locks;
	data->first_fork = philo->first_fork;
	data->second_fork = philo->second_fork;
	data->old_times_to_eat = philo->times_to_eat;
}

void	*death_timer(void *arg)
{
	t_thread_data	*data;
	int				old_eat;

	data = arg;
	end(1, data->end_lock);
	pthread_mutex_lock(data->struct_lock);
	old_eat = data->old_times_to_eat;
	pthread_mutex_unlock(data->struct_lock);
	usleep(data->die_time * 1000);
	pthread_mutex_lock(data->struct_lock);
	if (*data->times_to_eat != old_eat || *data->times_to_eat <= 0)
	{
		pthread_mutex_unlock(data->struct_lock);
		end(-1, data->end_lock);
		return (NULL);
	}
	pthread_mutex_unlock(data->struct_lock);
	end(-1, data->end_lock);
	if (!dead(0, data->dead_lock))
		lock_print(data->philo, data->dead_lock, data->print_lock, DIED);
	return (NULL);
}
