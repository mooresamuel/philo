/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:25:00 by samoore           #+#    #+#             */
/*   Updated: 2024/07/08 20:58:42 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

pthread_mutex_t	*get_fork_locks(t_type action, int num)
{
	static pthread_mutex_t	*fork_locks;
	int						i;

	if (action == INIT)
	{
		// fork_state(NULL, 0, NULL, INIT);
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
		// while (++i < philo)
		// 	pthread_mutex_destroy(&struct_locks[i]);
		free (struct_locks);
	}
	else
		return (&struct_locks[philo]);
	return (NULL);
}
