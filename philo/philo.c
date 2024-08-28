/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 12:16:41 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	*start_time(void)
{
	struct timeval	time;
	static long		start_time = 0;

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

void	*death_timer(void *arg)
{
	t_thread_data	*data;
	int				old_eat;

	data = arg;
	end(1, data->end_lock);
	pthread_mutex_lock(data->struct_lock);
	old_eat = data->old_times_to_eat;
	pthread_mutex_unlock(data->struct_lock);
	usleep(data->die_time * 1000);
	pthread_mutex_lock(data->struct_lock);
	if (*data->times_to_eat != old_eat || *data->times_to_eat <= 0)
	{
		pthread_mutex_unlock(data->struct_lock);
		end(-1, data->end_lock);
		return (NULL);
	}
	pthread_mutex_unlock(data->struct_lock);
	end(-1, data->end_lock);
	if (!dead(0, data->dead_lock))
		lock_print(data->philo, data->dead_lock, data->print_lock, DIED);
	return (NULL);
}

void	set_timer_data(t_thread_data *data, t_philos *philo)
{
	data->dead_lock = philo->dead_lock;
	data->end_lock = philo->end_lock;
	data->philo = philo->philo;
	data->print_lock = philo->print_lock;
	data->struct_lock = philo->struct_lock;
	data->times_to_eat = &philo->times_to_eat;
	data->die_time = philo->die_time;
	data->has_first_fork = &philo->has_first_fork;
	data->has_second_fork = &philo->has_second_fork;
	data->fork_locks = philo->fork_locks;
	data->first_fork = philo->first_fork;
	data->second_fork = philo->second_fork;
	data->old_times_to_eat = philo->times_to_eat;
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

	take_fork(philo, philo->first_fork);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, TAKEN_FORK);
	if (philo->first_fork == philo->second_fork)
	{
		return_fork(philo, philo->first_fork);
		usleep(philo->die_time * 1000);
	}
	take_fork(philo, philo->second_fork);
	decrement_eat_times(philo->struct_lock, &philo->times_to_eat);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, EATING);
	if (!dead(0, philo->dead_lock) && philo->times_to_eat)
	{
		pthread_mutex_lock(philo->struct_lock);
		data->old_times_to_eat = philo->times_to_eat;
		pthread_mutex_unlock(philo->struct_lock);
		pthread_create(&timer, NULL, &death_timer, (void *)data);
		pthread_detach(timer);
	}
	usleep(philo->eat_time * 1000);
	return_fork(philo, philo->first_fork);
	return_fork(philo, philo->second_fork);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, SLEEPING);
	usleep(philo->sleep_time * 1000);
	lock_print(philo->philo, philo->dead_lock, philo->print_lock, THINKING);
	usleep(500);
}

void	*new_philosopher(void *arg)
{
	t_thread_data	data;
	t_philos		*philo;
	pthread_t		timer;

	philo = arg;
	set_timer_data(&data, philo);
	philo->start_time = *start_time();
	pthread_create(&timer, NULL, &death_timer, (void *)&data);
	pthread_detach(timer);
	if (philo->philo % 2 == 0)
		usleep(1100);
	while (!dead(0, philo->dead_lock) && philo->times_to_eat)
		philo_main_loop(philo, &data);
	if (philo->has_first_fork)
		pthread_mutex_unlock(&philo->fork_locks[philo->first_fork]);
	if (philo->has_second_fork)
		pthread_mutex_unlock(&philo->fork_locks[philo->second_fork]);
	end(-1, philo->end_lock);
	while (end(0, philo->end_lock))
		usleep(1000);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int	start;
	t_philos	*philos;
	pthread_t	*tid;
	int			i;

	i = -1;
	start = 0;
	if (argc != 5 && argc != 6)
		return (printf("Invalid number of arguments!\n"), 1);
	tid = malloc(sizeof(pthread_t) * my_atoi(argv[1]));
	start_time();
	philos = init_philos(my_atoi(argv[1]), argc, argv, &start);
	i = -1;
	while (++i < my_atoi(argv[1]))
		pthread_create(&(tid[i]), NULL, &new_philosopher, (void *)&philos[i]);
	// start = 1;
	while (!dead(0, philos[0].dead_lock) && !end(0, philos[0].end_lock))
		usleep(100);
	while (--i >= 0)
		pthread_join(tid[i], NULL);
	get_struct_lock(CLEAN, 0);
	get_fork_locks(CLEAN, 0);
	free (philos);
	free (tid);
	return (0);
}
