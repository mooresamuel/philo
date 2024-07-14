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

long	get_time_since_start(int silent)
{
	struct timeval	time;
	long			elapsed_time;

	gettimeofday(&time, NULL);
	elapsed_time = (time.tv_sec * 1000 + time.tv_usec / 1000) - *start_time ();
	if (!silent)
		printf("%4ld : ", elapsed_time);
	return (elapsed_time);
}

int	lock_print(t_philos *philo, t_philo_state state)
{
	int	philo_num;

	// printf("waiting for print/n");
	pthread_mutex_lock(&philo->struct_lock);
	philo_num = philo->philo;
	pthread_mutex_unlock(&philo->struct_lock);
	// if (!dead(0, philo->dead, philo->dead_lock))
	// {
		// pthread_mutex_lock(philo->print_lock);
		if (philo->end)
			return (1);
		sem_wait(philo->print_lock);
		// if (dead(0, philo->dead, philo->dead_lock))
		// {
		// 	pthread_mutex_unlock(philo->print_lock);
		// 	return ;
		// }
		get_time_since_start(0);
		if (state == EATING)
			printf("philo %d is eating\n", philo_num + 1);
		else if (state == SLEEPING)
			printf("philo %d is sleeping\n", philo_num + 1);
		else if (state == THINKING)
			printf("philo %d is thinking\n", philo_num + 1);
		else if (state == TAKEN_FORK)
			printf("philo %d has taken a fork\n", philo_num + 1);
		else if (state == DIED)
		{
			printf("philo %d has died\n", philo_num + 1);
			// usleep(philo->die_time * 1000);
			// sem_close(philo->forks);
			// // sem_unlink(SEM_FORK);
			// sem_close(philo->end_lock);
			// // sem_unlink(SEM_END);
			// sem_close(philo->print_lock);
			// // sem_unlink(SEM_PRINT);
			sem_post(philo->end_lock);
			sem_post(philo->print_lock);
			philo->end = 1;
					// sem_post(philo->print_lock);
			return (1);
			// exit (1);
		}
		sem_post(philo->print_lock);
	// }
	return (0);
}

void	*death_timer(void *arg)
{
	t_thread_data	*data;
	int				old_eat;

	data = arg;
	pthread_mutex_lock(data->struct_lock);
	old_eat = data->old_times_to_eat;
	pthread_mutex_unlock(data->struct_lock);
	usleep(data->die_time * 1000);
	pthread_mutex_lock(data->struct_lock);
	if (*data->times_to_eat != old_eat || *data->times_to_eat <= 0)
	{
		pthread_mutex_unlock(data->struct_lock);
		return (NULL);
	}
	if (data->philos->end)
		return (NULL);
	pthread_mutex_unlock(data->struct_lock);
	// if (!dead(0, data->dead, data->dead_lock))
	// {
			// sem_close(data->philos->forks);
			// // sem_unlink(SEM_FORK);
			// sem_close(data->philos->end_lock);
			// // sem_unlink(SEM_END);
			// sem_close(data->philos->print_lock);
			// // sem_unlink(SEM_PRINT);
		// exit(1);
		lock_print(data->philos, DIED);
	// }
	return (NULL);
}

void	set_timer_data(t_thread_data *data, t_philos *philo)
{
	data->dead_lock = philo->dead_lock;
	data->philo = philo->philo;
	data->struct_lock = &philo->struct_lock;
	data->times_to_eat = &philo->times_to_eat;
	data->die_time = philo->die_time;
	data->has_first_fork = &philo->has_first_fork;
	data->has_second_fork = &philo->has_second_fork;
	data->old_times_to_eat = philo->times_to_eat;
	data->philos = philo;
}

void	decrement_eat_times(pthread_mutex_t *lock, int *num)
{
	pthread_mutex_lock(lock);
	(*num)--;
	pthread_mutex_unlock(lock);
}

void	philo_main_loop(t_philos *philo, t_thread_data *data)
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
	decrement_eat_times(&philo->struct_lock, &philo->times_to_eat);
	lock_print(philo, EATING);
	if (philo->times_to_eat && !philo->end)
	{
		pthread_mutex_lock(&philo->struct_lock);
		data->old_times_to_eat = philo->times_to_eat;
		pthread_mutex_unlock(&philo->struct_lock);
		pthread_create(&timer, NULL, &death_timer, (void *)data);
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
	sem_post(philo->end_lock);
	philo->end = 1;
	// sem_close(philo->forks);
	// sem_unlink(SEM_FORK);
	// sem_close(philo->print_lock);
	// sem_unlink(SEM_PRINT);
	// sem_close(philo->end_lock);
	// sem_unlink(SEM_END);
	// exit (1);
	return (NULL);
}

void	*new_philosopher(void *arg)
{
	t_thread_data	data;
	pthread_t		timer;
	pthread_t		end_check;
	t_philos		*philo;

	philo = arg;
	pthread_create(&end_check, NULL, &end_checker, arg);
	set_timer_data(&data, philo);
	// while (!*(philo->ready))
	// 	usleep(500);
	// while (dead(0, philo->dead, philo->dead_lock) == -1)
	// 	usleep(100);

	philo->start_time = *start_time();
	if (philo->philo % 2 == 0)
		usleep(1100);
	if (!philo->end)
	{
		pthread_create(&timer, NULL, &death_timer, (void *)&data);
		pthread_detach(timer);
	}
	// printf("times to eat:: %d\n", philo->times_to_eat);
	while (philo->times_to_eat && !philo->end)
		philo_main_loop(philo, &data);
	// if (philo->has_first_fork)
	// 	return_fork(philo);
	// while (end(0, philo->en
	// if (!philo->end)
	// 	sem_post(philo->end_lock);
	// philo->end = 1;
	pthread_join(end_check, NULL);
	sem_close(philo->forks);
	sem_unlink(SEM_FORK);
	sem_close(philo->print_lock);
	sem_unlink(SEM_PRINT);
	sem_close(philo->end_lock);
	sem_unlink(SEM_END);
	return (NULL);
}

void	child_process(t_philos *philo, int num, int *dead, pid_t *pid)
{
	pthread_t	new;

	philo->end = 0;
	pthread_mutex_init(&philo->struct_lock, NULL);
	philo->philo = num;
	// new_philosopher(philo);
	// printf(".......times to eat:: %d\n", philo->times_to_eat);
	pthread_create(&new, NULL, &new_philosopher, (void *)philo);
	pthread_join(new, NULL);
	sem_wait(philo->end_lock);
	sem_post(philo->end_lock);
	usleep(philo->die_time * 1000);
	philo->end = 1;
	// pthread_join(new, NULL);
		printf(" -----fin\n");
	// sem_wait(philo->end_lock);
	// sem_post(philo->end_lock);
	// usleep(philo->die_time * 2000);
	// printf("\n\n\nphilo %d done\n\n\n", num);
	sem_close(philo->forks);
	// sem_unlink(SEM_FORK);
	sem_close(philo->print_lock);
	// sem_unlink(SEM_PRINT);
	sem_close(philo->end_lock);
	free(pid);
	// sem_unlink(SEM_END);
	// kill (1, 2);
	exit (1);
}

int	main(int argc, char **argv)
{
	int			*dead_point;
	int			dead_val;
	t_philos	*philos;
	int			i;
	pid_t		*pid;

	i = -1;
	dead_val = -1;
	dead_point = &dead_val;
	sem_unlink(SEM_FORK);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_END);
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	pid = malloc(sizeof(pid_t) * my_atoi(argv[1]));
	start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv);
	sem_wait(philos->end_lock);
	i = -1;
	// dead(1, dead_point, philos->dead_lock);
	while (++i < my_atoi(argv[1]))
	{
		pid[i] = fork ();
		if (pid[i] == 0)
			child_process(philos, i, dead_point, pid);
		// else
		// {
		// 	sem_close(philos->forks);
		// 	sem_unlink(SEM_FORK);
		// 	sem_close(philos->print_lock);
		// 	sem_unlink(SEM_PRINT);
		// 	sem_close(philos->end_lock);
		// 	sem_unlink(SEM_END);	
		// }

	}
		// pthread_create(&(tid[i]), NULL, &new_philosopher, (void *)&philos[i]);
	// start = 1;
	// while (!dead(0, philo->dead, philos[0].dead_lock) && !end(0, philos[0].end_lock))
	// 	usleep(100);
	wait(0);
	// sem_post(philos->end_lock);
	printf("fin\n");
	// while (--i >= 0)
	// 	waitpid(pid[i], NULL, 0);
	// while (--i >= 0)
	// 	kill(pid[i], 2);
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
