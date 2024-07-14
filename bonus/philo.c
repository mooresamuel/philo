/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/07/10 19:27:36 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	*start_time(void)
{
	static long	start_time = 0;
	struct timeval		time;

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

int	lock_print(t_philos *philo, t_philo_state state)
{
	int	philo_num;
	long time;
	int first_dead;

	first_dead = 0;
	// printf("waiting for print/n");
	philo_num = philo->philo;
	// if (philo->end)		return (1);
	// if (!dead(0, philo->dead, philo->dead_lock))
	// {
		// pthread_mutex_lock(philo->print_lock);
		// if (dead(0, philo->dead, philo->dead_lock))
		// {
		// 	pthread_mutex_unlock(philo->print_lock);
		// 	return ;
		// }
		sem_wait(philo->print_lock);
		if (philo->end)
		{
			sem_post(philo->print_lock);
			return (1);
		}
		time = get_time_since_start();
		// usleep (3000);
		if (state == EATING && !philo->end)
			printf("%ld philo %d is eating\n",time, philo_num + 1);
		else if (state == SLEEPING && !philo->end)
			printf("%ld philo %d is sleeping\n",time, philo_num + 1);
		else if (state == THINKING && !philo->end)
			printf("%ld philo %d is thinking\n",time, philo_num + 1);
		else if (state == TAKEN_FORK && !philo->end)
			printf("%ld philo %d has taken a fork\n",time, philo_num + 1);
		else if (state == DIED)
		{
			// usleep(10000);
			sem_post(philo->end_lock);
			if (!philo->end)
				printf("%ld philo %d has died\n",time, philo_num + 1);
			return_fork(philo);
			usleep(15000);
			sem_post(philo->print_lock);
			return (1);
		}
		sem_post(philo->print_lock);
	return (0);
}

void	*death_timer(void *arg)
{
	t_philos		*philo;
	int				old_eat;

	philo = arg;
	old_eat = philo->times_to_eat;
	usleep(philo->die_time * 1000);
	// printf("philo %d die time %d\n", philo->philo, philo->die_time);
	if (philo->times_to_eat != old_eat || philo->times_to_eat <= 0)
	{
		return (NULL);
	}
	// if (philo->end)
	// 	return (NULL);
	if (!philo->end)
	{
		lock_print(philo, DIED);
	}
	return (NULL);
}

void	philo_main_loop(t_philos *philo)
{
	pthread_t		timer;

	take_fork(philo);
	lock_print(philo, TAKEN_FORK);
	if (philo->num_philos == 1)
	{
		return_fork(philo);
		usleep(philo->die_time * 1000);
	}
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

void	*end_checker(void *arg)
{
	t_philos	*philo;

	philo = arg;
	sem_wait(philo->end_lock);
	philo->end = 1;
	sem_post(philo->end_lock);
	return (NULL);
}

void	child_process(t_philos *philo, int num)
{
	pthread_t	new;
	pthread_t		timer;
	pthread_t		end_check;

	philo->end = 0;
	philo->philo = num;
	pthread_create(&end_check, NULL, &end_checker, (void*)philo);
	philo->start_time = *start_time();
	if (philo->philo % 2 == 0)
		usleep(1100);
	if (!philo->end)
		pthread_create(&timer, NULL, &death_timer, (void*)philo);
	while (philo->times_to_eat && !philo->end)
		philo_main_loop(philo);
	sem_post(philo->end_lock);
	pthread_join(timer, NULL);
	pthread_join(end_check, NULL);
	usleep(philo->die_time * 1000);
}

int	main(int argc, char **argv)
{
	t_philos	*philos;
	int			i;
	pid_t		*pid;

	i = -1;
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_END);
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	pid = malloc(sizeof(pid_t) * my_atoi(argv[1]));
	start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv);
	sem_wait(philos->end_lock);
	sem_post(philos->end_lock);
	sem_wait(philos->end_lock);
	i = -1;
	while (++i < my_atoi(argv[1]))
	{
		pid[i] = fork ();
		if (pid[i] == 0)
		{
			child_process(philos, i);
			free (pid);
			sem_close(philos->forks);
			sem_unlink(SEM_FORK);
			sem_close(philos->print_lock);
			sem_unlink(SEM_PRINT);
			sem_close(philos->end_lock);
			sem_unlink(SEM_END);
			exit (1);
		}
		// else
		// 	waitpid(pid[i], NULL, 0);
	}
		// pthread_create(&(tid[i]), NULL, &new_philosopher, (void *)&philos[i]);
	// start = 1;
	// while (!dead(0, philo->dead, philos[0].dead_lock) && !end(0, philos[0].end_lock))
	// 	usleep(100);
	// wait(0);
	// sem_post(philos->end_lock);
	// printf("fin\n");
	while (--i >= 0)
		waitpid(pid[i], NULL, 0);
	// while (--i >= 0)
	// 	kill(pid[i], 9);
	sem_close(philos->forks);
	sem_unlink(SEM_FORK);
	sem_close(philos->end_lock);
	sem_unlink(SEM_END);
	sem_close(philos->print_lock);
	sem_unlink(SEM_PRINT);
	free (pid);
	// free (philos);
	return (0);
}
