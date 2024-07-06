/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:14:24 by samoore           #+#    #+#             */
/*   Updated: 2024/07/06 14:28:57 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

pthread_mutex_t	*get_struct_lock(int philo, int init)
{
	static pthread_mutex_t	*struct_locks = NULL;
	int						i;

	if (init == 1)
	{
		struct_locks = malloc(sizeof(pthread_mutex_t) * philo);
		i = -1;
		while (++i < philo)
		{
			if (pthread_mutex_init(&struct_locks[i], NULL) != 0) { 
				printf("\n mutex init has failed\n"); 
				return NULL; 
			}
		}
	}
	else if (init == -1)
	{
		i = -1;
		while (++i < philo)
			pthread_mutex_destroy(&struct_locks[i]);
		free (struct_locks);
	}
	else 
	{
		return (&struct_locks[philo]);
	}
	return (NULL);
}

void	*pointer_to(e_type type)
{
	static pthread_mutex_t	print_lock;
	static pthread_mutex_t	dead_lock;
	static pthread_mutex_t	end_lock;

	if (type == PRINT_LOCK)
		return ((void*)&print_lock);
	if (type == DEAD_LOCK)
		return ((void*)&dead_lock);
	if (type == END_LOCK)
		return ((void*)&end_lock);
	if (type == INIT)
	{
		if (pthread_mutex_init(&print_lock, NULL) != 0) { 
			printf("\n mutex init has failed\n"); 
		return NULL; 
	}
		if (pthread_mutex_init(&dead_lock, NULL) != 0) { 
			printf("\n mutex init has failed\n"); 
		return NULL; 
	}
		if (pthread_mutex_init(&end_lock, NULL) != 0) { 
			printf("\n mutex init has failed\n"); 
		return NULL; 
	}
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
		philo->times_to_eat = 2147483647;
	else
		philo->times_to_eat = my_atoi(argv[5]);
}

t_philos	*init_philos(int num_philos, int argc, char **argv)
{
	struct timeval		s_time;
	long				start_time;
	int				i;
	t_philos		*philos;

	philos = malloc(sizeof(t_philos) * num_philos);

		// gettimeofday(&s_time, NULL);
		// start_time = ((long)s_time.tv_sec * 1000) + ((long)s_time.tv_usec / 1000);
		

	pointer_to(INIT);
	i = -1;
	get_struct_lock(num_philos, 1);
	while (++i < num_philos)
	{
		philos[i].forks = get_forks(num_philos);
		philos[i].print_lock = (pthread_mutex_t*)pointer_to(PRINT_LOCK);
		philos[i].dead_lock = (pthread_mutex_t*)pointer_to(DEAD_LOCK);
		philos[i].end_lock = (pthread_mutex_t*)pointer_to(END_LOCK);
		philos[i].struct_lock = get_struct_lock(i, 0);
		philos[i].philo = i;
		philos[i].num_philos = my_atoi(argv[1]);
		philos[i].first_fork = first_fork(num_philos, i);
		philos[i].second_fork = second_fork(num_philos, i);
		get_times(&philos[i], argc, argv);
	}
	return (philos);
}
