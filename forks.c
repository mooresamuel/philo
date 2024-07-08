/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:50:50 by samoore           #+#    #+#             */
/*   Updated: 2024/07/08 20:43:40 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

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

// int	fork_state(t_philos *philo, int num, int *forks, t_type action)
// {
// 	static pthread_mutex_t	lock;

// 	if (action == INIT)
// 	{
// 		pthread_mutex_init(&lock, NULL);
// 		return (0);
// 	}
// 	if (action == TAKE)
// 	{
// 		if (!philo->forks[num])
// 		{
// 			pthread_mutex_lock(&philo->fork_locks[num]);
// 			pthread_mutex_lock(&lock);
// 			philo->forks[num] = 1;
// 			pthread_mutex_unlock(&lock);
// 			return (1);
// 		}
// 		pthread_mutex_unlock(&lock);
// 		return (0);
// 	}
// 	else if (action == RETURN)
// 	{
// 		philo->forks[num] = 0;
// 		pthread_mutex_unlock(&lock);
// 	}
// 	return (0);
// }

void	return_fork(t_philos *philo, int fork)
{
	pthread_mutex_unlock(&philo->fork_locks[fork]);
	// pthread_mutex_lock(philo->struct_lock);
	if (fork == philo->first_fork)
		philo->has_first_fork = 0;
	else
		philo->has_second_fork = 0;
	// pthread_mutex_unlock(philo->struct_lock);
	// fork_state(philo, fork, philo->forks, RETURN);
}

int	take_fork(t_philos *philo, int fork)
{
	int	state;
	pthread_mutex_t *lock;

	// state = fork_state(philo, fork, forks, TAKE);
	// while (!state)
	// {
	// 	usleep(100);
	// 	state = fork_state(philo, fork, forks, TAKE);
	// }
	// if (!state && dead(0, philo->dead_lock))
	// {
	// 	if (philo->has_first_fork)
	// 		return_fork(philo, philo->first_fork);
	// 	if (philo->has_second_fork)
	// 		return_fork(philo, philo->second_fork);
	// 	return (0);
	// }
	pthread_mutex_lock(&philo->fork_locks[fork]);
	// pthread_mutex_lock(philo->struct_lock);
	if (fork == philo->first_fork)
		philo->has_first_fork = 1;
	else
		philo->has_second_fork = 1;
	lock = &philo->fork_locks[fork];
	// pthread_mutex_unlock(philo->struct_lock);
	return (1);
}

