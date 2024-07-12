/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/07/12 17:12:24 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	// printf("waiting for print/n");
	// pthread_mutex_lock(&philo->struct_lock);
	philo_num = philo->philo;
	// pthread_mutex_unlock(&philo->struct_lock);
	// if (!dead(0, philo->dead, philo->dead_lock))
	// {
		// pthread_mutex_lock(philo->print_lock);
		// if (philo->end)
		// 	return (1);
			sem_wait(philo->print_lock);
		// usleep(1000);
		// if (philo->end)
		// {
		// 	sem_post(philo->print_lock);			
		// 	return (1);
		// }
		// if (dead(0, philo->dead, philo->dead_lock))
		// {
		// 	pthread_mutex_unlock(philo->print_lock);
		// 	return ;
		// }
		time = get_time_since_start();
		// if (philo->end)
		// {
		// 	sem_post(philo->print_lock);
		// 	return (1);
		// }
		if (state == EATING && !philo->end)
			printf("%ld philo %d is eating\n", time, philo_num + 1);
		else if (state == SLEEPING && !philo->end)
			printf("%ld philo %d is sleeping\n", time, philo_num + 1);
		else if (state == THINKING && !philo->end)
			printf("%ld philo %d is thinking\n", time, philo_num + 1);
		else if (state == TAKEN_FORK && !philo->end)
			printf("%ld philo %d has taken a fork\n", time, philo_num + 1);
		else if (state == DIED && !philo->end)
		{
			// if (philo->end)
			// {
			// 	sem_post(philo->print_lock);
			// 	sem_post(philo->end_lock);
			// 	return (1);
			// }
			if (!philo->end)
				printf("%ld philo %d has died\n", time, philo_num + 1);

			sem_post(philo->print_lock);
			sem_post(philo->end_lock);
			return_fork(philo);
			// usleep(philo->die_time * 1000);
			// sem_close(philo->forks);
			// sem_unlink(SEM_FORK);
			// sem_close(philo->end_lock);
			// sem_unlink(SEM_END);
			// sem_close(philo->print_lock);
			// sem_unlink(SEM_PRINT);
			// philo->end = 1;
			// sem_post(philo->print_lock);
					// sem_post(philo->print_lock);
			// exit (1);
			// exit (1);
			philo->wait = 0;
			return (0);
		}
		sem_post(philo->print_lock);
	// }
	return (0);
}

void	*death_timer(void *arg)
{
	t_philos	*philo;
	atomic_int				old_eat;


	philo = (t_philos *)arg;
	atomic_int	die_time = philo->die_time;

	printf("philo %d      die time: %d\n", philo->philo, philo->die_time);	

	// pthread_mutex_lock(data->struct_lock);
	old_eat = philo->times_to_eat;
	// pthread_mutex_unlock(philo->struct_lock);
	// printf("die time: %d\n", die_time);
	usleep(die_time * 1000);
	// pthread_mutex_lock(philo->struct_lock);
	// if (philo->end)
	// 	return (NULL);
	if (philo->times_to_eat != old_eat || philo->times_to_eat <= 0)
	{
		// pthread_mutex_unlock(data->struct_lock);
		return (NULL);
	}
		// philo->end = 1;
		philo->wait = 1;
	return(lock_print(philo, DIED), NULL);
	// }
	return (NULL);
}

void	philo_main_loop(t_philos *philo)
{	
	pthread_t	new;
	t_timer_data	data;

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
	if (philo->times_to_eat && !philo->end && !philo->wait)
	{
		data.philo = philo;
		data.die_time = philo->die_time;
		data.old_eat = philo->times_to_eat;
		pthread_create(&new, NULL, &death_timer, (void*)&data);
		pthread_detach(new);
	}
	usleep(philo->eat_time * 1000);
	return_fork(philo);
	lock_print(philo, SLEEPING);
	usleep(philo->sleep_time * 1000);
	lock_print(philo, THINKING);
	usleep(500);
	// pthread_join(*timer, NULL);
	return ;
}

void	*end_checker(void *arg)
{
	t_philos	*philo;

	philo = arg;
	sem_wait(philo->end_lock);
	sem_post(philo->end_lock);
	philo->end = 1;
	// printf("END CHECKER\n");
	return (NULL);
}

void	*new_philosopher(void *arg)
{
	static pthread_t		timer;
	pthread_t		new;
	t_philos		*philo;

	philo = arg;
	philo->wait = 0;
	philo->start_time = *start_time();
	printf("philo %d      die time: %d\n", philo->philo, philo->die_time);	
	if (philo->philo % 2 == 1)
		usleep(10000);
	if (!philo->end)
	{
		pthread_create(&timer, NULL, &death_timer, (void *)&philo);
	}
	while (philo->times_to_eat && !philo->end)
	{
		philo_main_loop(philo);
		// timer = new;
	}
	// if (new)
		// pthread_join(new, NULL);
	while (philo->wait)
		usleep(1000);
	// pthread_join(timer, NULL);
	// return_fork(philo);
	// sem_post(philo->end_lock);
	return (NULL);
}

void	child_process(t_philos *philo, int num)
{
	pthread_t	new;
	pthread_t		end_check;
	atomic_int			die_time;

	philo->end = 0;
	die_time = philo->die_time;
	philo->philo = num;
	pthread_create(&end_check, NULL, &end_checker, (void*)philo);
	pthread_create(&new, NULL, &new_philosopher, (void*)philo);
	pthread_join(new, NULL);
	pthread_join(end_check, NULL);
	// sem_post(philo->end_lock);
	// usleep(philo->die_time * 1000);
	sem_close(philo->forks);
	sem_close(philo->print_lock);
	sem_close(philo->end_lock);
	return ;
}

int	main(int argc, char **argv)
{
	t_philos	*philos;
	atomic_int			i;
	atomic_int		*pid;

	i = -1;
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_END);
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	pid = malloc(sizeof(atomic_int) * my_atoi(argv[1]));
	// start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv);
	// sem_post(philos->end_lock);
	sem_wait(philos->end_lock);
	// sem_wait(philos->end_lock);
	i = -1;
	while (++i < my_atoi(argv[1]))
	{
		pid[i] = fork ();
		if (pid[i] == 0)
		{
			child_process(philos, i);
			free (pid);
			exit (0);
		}

	}
	while (--i >= 0)
		waitpid(pid[i], NULL, 0);
	sem_close(philos->forks);
	sem_unlink(SEM_FORK);
	sem_close(philos->end_lock);
	sem_unlink(SEM_END);
	sem_close(philos->print_lock);
	sem_unlink(SEM_PRINT);
	free (pid);
	return (0);
}
