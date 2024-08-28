/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 13:14:24 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 12:36:28 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

t_philos	*init_philos(int num_philos, int argc, char **argv)
{
	static t_philos	philos;
	static sem_t	*forks;
	static sem_t	*print_lock;
	static sem_t	*end_lock;

	forks = sem_open(SEM_FORK, O_CREAT | O_EXCL, 0644, num_philos);
	print_lock = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 0644, 1);
	end_lock = sem_open(SEM_END, O_CREAT | O_EXCL, 00070, 1);
	philos.forks = forks;
	philos.print_lock = print_lock;
	philos.end_lock = end_lock;
	philos.num_philos = my_atoi(argv[1]);
	philos.has_first_fork = 0;
	philos.has_second_fork = 0;
	philos.die_time = my_atoi(argv[2]);
	philos.eat_time = my_atoi(argv[3]);
	philos.sleep_time = my_atoi(argv[4]);
	if (argc == 5)
		philos.times_to_eat = INT_MAX;
	else
		philos.times_to_eat = my_atoi(argv[5]);
	return (&philos);
}
