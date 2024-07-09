/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:50:50 by samoore           #+#    #+#             */
/*   Updated: 2024/07/08 21:32:31 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	first_fork(int num_philos, int philo)
{
	if (philo < (philo + 1) % num_philos)
		return (philo);
	else
		return (0);
}

int	second_fork(int num_philos, int philo)
{
	if (philo < (philo + 1) % num_philos)
		return ((philo + 1) % num_philos);
	else
		return (philo);
}

void	return_fork(t_philos *philo)
{
	int got_fork;

	got_fork = 0;
	pthread_mutex_lock(philo->struct_lock);
	if (philo->has_first_fork)
	{
		if (philo->has_first_fork)
		{
			got_fork = 1;
			philo->has_first_fork = 0;
		}
	}
	else if(philo->has_second_fork)
	{
		if (philo->has_second_fork)
		{
			got_fork = 1;
			philo->has_second_fork = 0;
		}
	}	
	pthread_mutex_unlock(philo->struct_lock);
	if (!got_fork)
		return ;
	sem_post(philo->forks);
}

int	take_fork(t_philos *philo)
{
	sem_wait(philo->forks);
	pthread_mutex_lock(philo->struct_lock);
	if (!philo->has_first_fork)
		philo->has_first_fork = 1;
	else
		philo->has_second_fork = 1;
	pthread_mutex_unlock(philo->struct_lock);
	return (1);
}

