/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:50:50 by samoore           #+#    #+#             */
/*   Updated: 2024/07/05 21:22:18 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

pthread_mutex_t	*get_forks(int num_philos)
{
	static pthread_mutex_t	*forks = NULL;
	int						i;
	static int				num;

	if (num_philos > 0)
		num = num_philos;
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
	{
		i = 0;
		// while (i < num)
		// 	pthread_mutex_destroy(&forks[i++]);
	// pthread_mutex_destroy(forks);
	free (forks);
	return (NULL);
	}
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
