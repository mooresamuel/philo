/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:09:04 by samoore           #+#    #+#             */
/*   Updated: 2024/11/27 14:06:42 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

long	get_time_since_start(void)
{
	struct timeval	time;
	long			elapsed_time;

	gettimeofday(&time, NULL);
	elapsed_time = (time.tv_sec * 1000 + time.tv_usec / 1000) - *start_time ();
	return (elapsed_time);
}

void	*death_timer(void *arg)
{
	t_philos		*philo;
	int				old_eat;

	philo = arg;
	old_eat = philo->times_to_eat;
	usleep(philo->die_time * 1000);
	if (philo->times_to_eat != old_eat || philo->times_to_eat <= 0)
	{
		return (NULL);
	}
	if (!philo->end)
	{
		lock_print(philo, DIED);
	}
	return (NULL);
}

void	*end_checker(void *arg)
{
	t_philos	*philo;

	philo = arg;
	sem_wait(philo->end_lock);
	philo->end = 1;
	sem_post(philo->end_lock);
	return (NULL);
}
