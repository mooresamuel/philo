/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:14:24 by samoore           #+#    #+#             */
/*   Updated: 2024/07/08 16:26:27 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

pthread_mutex_t	*get_struct_lock(int philo, t_type action)
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
		while (++i < philo)
			pthread_mutex_destroy(&struct_locks[i]);
		free (struct_locks);
	}
	else
		return (&struct_locks[philo]);
	return (NULL);
}

void	*pointer_to(t_type type)
{
	static pthread_mutex_t	print_lock;
	static pthread_mutex_t	dead_lock;
	static pthread_mutex_t	end_lock;

	if (type == PRINT_LOCK)
		return ((void *) &print_lock);
	if (type == DEAD_LOCK)
		return ((void *) &dead_lock);
	if (type == END_LOCK)
		return ((void *) &end_lock);
	if (type == INIT)
	{
		pthread_mutex_init(&print_lock, NULL);
		pthread_mutex_init(&dead_lock, NULL);
		pthread_mutex_init(&end_lock, NULL);
	}
	return (NULL);
}

int	my_atoi(char *str)
{
	int	num;

	num = 0;
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (num);
		num *= 10;
		num += *str++ - '0';
	}
	return (num);
}

void	get_times(t_philos *philo, int argc, char **argv)
{
	philo->die_time = my_atoi(argv[2]);
	philo->eat_time = my_atoi(argv[3]);
	philo->sleep_time = my_atoi(argv[4]);
	if (argc == 5)
		philo->times_to_eat = INT_MAX;
	else
		philo->times_to_eat = my_atoi(argv[5]);
}

pthread_mutex_t	*get_fork_locks(t_type action, int num)
{
	static pthread_mutex_t	*fork_locks;
	int						i;

	if (action == INIT)
	{
		fork_state(NULL, 0, NULL, INIT);
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

t_philos	*init_philos(int num_philos, int argc,
						char **argv, atomic_int *start)
{
	int					i;
	t_philos			*philos;

	philos = malloc(sizeof(t_philos) * num_philos);
	pointer_to(INIT);
	i = -1;
	get_struct_lock(num_philos, INIT);
	get_fork_locks(INIT, num_philos);
	while (++i < num_philos)
	{
		philos[i].ready = start;
		philos[i].fork_locks = get_fork_locks(RETURN, 0);
		philos[i].forks = get_forks(0);
		philos[i].print_lock = (pthread_mutex_t *)pointer_to(PRINT_LOCK);
		philos[i].dead_lock = (pthread_mutex_t *)pointer_to(DEAD_LOCK);
		philos[i].end_lock = (pthread_mutex_t *)pointer_to(END_LOCK);
		philos[i].struct_lock = get_struct_lock(i, RETURN);
		philos[i].philo = i;
		philos[i].num_philos = my_atoi(argv[1]);
		philos[i].first_fork = first_fork(num_philos, i);
		philos[i].second_fork = second_fork(num_philos, i);
		philos[i].has_first_fork = 0;
		philos[i].has_second_fork = 0;
		get_times(&philos[i], argc, argv);
	}
	end(num_philos, (pthread_mutex_t *)pointer_to(END_LOCK));
	return (philos);
}
