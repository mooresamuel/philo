	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   philo.c                                            :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2024/07/01 18:18:19 by samoore           #+#    #+#             */
	/*   Updated: 2024/07/04 16:42:42 by samoore          ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */

	#include <philo.h>

	atomic_long	*start_time(void)
	{
		static atomic_long	start_time = 0;
		struct		timeval time;

		if (!start_time)
		{
			gettimeofday(&time, NULL);
			start_time = ((long)time.tv_sec * 1000 + (long)time.tv_usec / 1000);
		}
		return (&start_time);
	}

	long	get_time_since_start(int silent)
	{
		struct	timeval time;
		long			elapsed_time;

		gettimeofday(&time, NULL);
		elapsed_time = (time.tv_sec * 1000 + time.tv_usec / 1000) - *start_time ();
		if (!silent)
			printf("%4ld : ", elapsed_time);
		return (elapsed_time);
	}

	void	lock_print(int philo, pthread_mutex_t *dead_lock, pthread_mutex_t *print_lock, e_philo_state state)
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
				printf("philo %d is eating\n", philo);
			else if (state == SLEEPING)
				printf("philo %d is sleeping\n", philo);
			else if (state == THINKING)
				printf("philo %d is thinking\n", philo);
			else if (state == TAKEN_FORK)
				printf("philo %d has taken a fork\n", philo); 
			else if (state == DIED)
			{
				dead(1, dead_lock);
				printf("philo %d has died\n", philo);
			}
			else if (state == LIVE)
				printf("\nThank you for not starving us to death <3\n");
			pthread_mutex_unlock(print_lock);
		}
	}

	int	max(int a, int b)
	{
		if (a > b)
			return (a);
		return (b);
	}

	void	*start_timer(void *arg)
	{
		t_thread_data	*data;
		int				old_eat;
		pthread_mutex_t *lock;

		data = arg;
		old_eat = data->old_times;
		end(1, data->end_lock);
		lock = data->struct_lock;
		if (dead(0, data->dead_lock))
		{
			end(-1, data->end_lock);
			return (NULL);
		}
		pthread_mutex_lock(lock);
		pthread_mutex_unlock(lock);
		usleep(data->die_time * 1000);
		if (dead(0, data->dead_lock))
		{
			end(-1, data->end_lock);
			return (NULL);
		}
		pthread_mutex_lock(lock);
		// printf("%d old %d *data \n", old_eat, *data->times_to_eat);	
		if (*data->times_to_eat != old_eat || *data->times_to_eat <= 0)
		{
			pthread_mutex_unlock(lock);
			end(-1, data->end_lock);
			return (NULL);
		}
		// pthread_detach(data->this);
		pthread_mutex_unlock(lock);
		if (!dead(0, data->dead_lock))
		lock_print(data->philo, data->dead_lock, data->print_lock, DIED);
		end(-1, data->end_lock);
		return (NULL);
	}

	void *new_philosopher(void *arg) 
	{
		t_philos	*philo;
		pthread_t	timer;
		int			id;
		int			eat_time;
		int			sleep_time;
		pthread_mutex_t *lock;
		t_thread_data data;

		
		philo = arg;
		// pthread_mutex_init(philo->data_lock, NULL);
		data.data_lock = philo->data_lock;
		data.dead_lock = philo->dead_lock;
		data.end_lock = philo->end_lock;
		data.philo = philo->philo;
		data.print_lock = philo->print_lock;
		data.struct_lock = philo->struct_lock;
		data.times_to_eat = &philo->times_to_eat;
		data.die_time = philo->die_time;
		lock = philo->struct_lock;
		sleep_time = philo->sleep_time;
		eat_time = philo->eat_time;
		id = philo->philo;
		while (!*(philo->ready))
			usleep(1);
		philo->start_time = *start_time();
		data.old_times = philo->times_to_eat;
		if (dead(0, philo->dead_lock))
			return (NULL);
		pthread_create(&timer, NULL, &start_timer, (void*)&data);
		pthread_detach(timer);
		if (id % 2 == 1)
			usleep(10000);

		while (!dead(0, philo->dead_lock) && philo->times_to_eat)
		{
			if (philo->first_fork == philo->second_fork)
				continue ;
			lock_print(philo->philo, philo->dead_lock, philo->print_lock, THINKING);

			pthread_mutex_lock(&philo->forks[philo->first_fork]);

			
			lock_print(philo->philo, philo->dead_lock, philo->print_lock, TAKEN_FORK);

			pthread_mutex_lock(&philo->forks[philo->second_fork]);

			pthread_mutex_lock(lock);
			philo->times_to_eat--;
			pthread_mutex_unlock(lock);
			lock_print(philo->philo, philo->dead_lock, philo->print_lock, EATING);
			data.old_times = philo->times_to_eat;
				pthread_create(&timer, NULL, &start_timer, (void*)&data);
				pthread_detach(timer);

			if (!dead(0, philo->dead_lock))
				usleep(eat_time * 1000);
			pthread_mutex_lock(lock);

			pthread_mutex_unlock(lock);
			// if (!dead(0, philo->dead_lock))
			// {
			// }
			pthread_mutex_unlock(&philo->forks[philo->first_fork]);
			pthread_mutex_unlock(&philo->forks[philo->second_fork]);
			lock_print(philo->philo, philo->dead_lock, philo->print_lock, SLEEPING);
			// printf("%d sleep\n", sleep_time);
			if (!dead(0, philo->dead_lock))
				usleep(sleep_time * 1000);
			// usleep(5);
		}
		end(-1, philo->end_lock);
		while(end(0, philo->end_lock))
			usleep(1);
		usleep(philo->die_time);
		return NULL; 
	} 

	int main(int argc, char **argv) 
	{ 
		int i = -1; 
		int error;
		t_philos *philos;
		pthread_t *tid;
		atomic_int start = 0;
		pthread_mutex_t dead_lock;
		pthread_mutex_t end_lock;

		pthread_mutex_init(&dead_lock, NULL);
		pthread_mutex_init(&end_lock, NULL);
		int	threads;

		if (argc != 5 && argc != 6)
			return (printf("Invalid number of arguments!\n"), 1);

		tid = malloc(sizeof(pthread_t) * my_atoi(argv[1]));
		philos = init_philos(my_atoi(argv[1]), argc, argv);
		end(my_atoi(argv[1]), &end_lock);
		i = 0;
		while (i < my_atoi(argv[1]))
		{
			philos[i].ready = &start;
			philos[i].dead_lock = &dead_lock;
			philos[i].end_lock = &end_lock;
			philos[i].tid = tid[i];
			if (dead(0, philos[i].dead_lock))
				break ;
			error = pthread_create(&(tid[i]), NULL, &new_philosopher, (void*)&philos[i]);
			if (error) 
				break ;
			i++; 
		}
		threads = i;
		start_time();
		start = 1;
		while (!end(0, &end_lock))
			usleep(1000);
		i = 0;

		while (i < threads)
		{
			pthread_join(tid[i], NULL);
			i++;
		}
		// usleep(10000000);
		get_struct_lock(0, -1);
		get_forks(-1);
		free (philos);
		free (tid);
		return 0; 
	} 