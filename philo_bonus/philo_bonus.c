/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/11/27 14:06:21 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_main_loop(t_philos *philo)
{
	pthread_t		timer;

	take_fork(philo);
	lock_print(philo, TAKEN_FORK);
	take_fork(philo);
	philo->times_to_eat--;
	lock_print(philo, EATING);
	if (philo->times_to_eat && !philo->end)
	{
		pthread_create(&timer, NULL, &death_timer, (void *)philo);
		pthread_detach(timer);
	}
	usleep(philo->eat_time * 1000);
	return_fork(philo);
	lock_print(philo, SLEEPING);
	usleep(philo->sleep_time * 1000);
	lock_print(philo, THINKING);
	usleep(500);
}

void	child_process(t_philos *philo, int num)
{
	pthread_t	timer;
	pthread_t	end_check;

	philo->end = 0;
	philo->philo = num;
	pthread_create (&end_check, NULL, &end_checker, (void *)philo);
	philo->start_time = *start_time();
	if (philo->philo % 2 == 0)
		usleep(1100);
	if (!philo->end)
		pthread_create (&timer, NULL, &death_timer, (void *)philo);
	while (philo->times_to_eat && !philo->end)
		philo_main_loop(philo);
	sem_post(philo->end_lock);
	pthread_join(timer, NULL);
	pthread_join(end_check, NULL);
	usleep(philo->die_time * 1000);
}

int	create_philosopher(t_philos *philos, int i, pid_t *pid_to_free)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		child_process(philos, i);
		close_sem(philos);
		free (pid_to_free);
		exit(1);
	}
	return (pid);
}

int	check_input(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	while (++i < argc)
	{
		if (my_atoi(argv[i]) < 0)
		{
			printf("Invalid argument: %s\n", argv[i]);
			return (1);
		}
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_philos	*philos;
	int			i;
	pid_t		*pid;

	i = -1;
	unlink_sem();
	if (check_input(argc, argv))
		return (1);
	pid = malloc(sizeof(pid_t) * my_atoi(argv[1]));
	start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv);
	sem_wait(philos->end_lock);
	i = -1;
	while (++i < my_atoi(argv[1]))
		pid[i] = create_philosopher(philos, i, pid);
	while (--i >= 0)
		waitpid(pid[i], NULL, 0);
	close_sem(philos);
	free (pid);
	return (0);
}
