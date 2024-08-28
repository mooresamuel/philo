/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:25:00 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 14:52:24 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*get_fork_locks(t_type action, int num)
{
	static pthread_mutex_t	*fork_locks;
	int						i;

	if (action == INIT)
	{
		i = 0;
		fork_locks = malloc(sizeof(pthread_mutex_t) * num);
		while (i < num)
			pthread_mutex_init(&fork_locks[i++], NULL);
	}
	if (action == CLEAN)
	{
		free (fork_locks);
		return (NULL);
	}
	return (fork_locks);
}

pthread_mutex_t	*get_struct_lock(t_type action, int philo)
{
	static pthread_mutex_t	*struct_locks = NULL;
	int						i;

	if (action == INIT)
	{
		struct_locks = malloc(sizeof(pthread_mutex_t) * philo);
		i = -1;
		while (++i < philo)
			pthread_mutex_init(&struct_locks[i], NULL);
	}
	else if (action == CLEAN)
	{
		i = -1;
		free (struct_locks);
	}
	else
		return (&struct_locks[philo]);
	return (NULL);
}

void	lock_print(int philo, pthread_mutex_t *dead_lock,
					pthread_mutex_t *print_lock, t_philo_state state)
{
	if (!dead(0, dead_lock))
	{
		pthread_mutex_lock(print_lock);
		if (dead(0, dead_lock))
		{
			pthread_mutex_unlock(print_lock);
			return ;
		}
		get_time_since_start(0);
		if (state == EATING)
			printf("philo %d is eating\n", philo + 1);
		else if (state == SLEEPING)
			printf("philo %d is sleeping\n", philo + 1);
		else if (state == THINKING)
			printf("philo %d is thinking\n", philo + 1);
		else if (state == TAKEN_FORK)
			printf("philo %d has taken a fork\n", philo + 1);
		else if (state == DIED)
		{
			dead(1, dead_lock);
			printf("philo %d has died\n", philo + 1);
		}
		pthread_mutex_unlock(print_lock);
	}
}
