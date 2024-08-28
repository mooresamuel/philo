/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 13:18:46 by samoore           #+#    #+#             */
/*   Updated: 2024/08/28 13:50:41 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print_action(t_philo_state state, long time, int philo_num)
{
	if (state == EATING)
		printf("%ld philo %d is eating\n", time, philo_num + 1);
	else if (state == SLEEPING)
		printf("%ld philo %d is sleeping\n", time, philo_num + 1);
	else if (state == THINKING)
		printf("%ld philo %d is thinking\n", time, philo_num + 1);
	else if (state == TAKEN_FORK)
		printf("%ld philo %d has taken a fork\n", time, philo_num + 1);
}

int	print_dead(t_philos *philo, long time, int philo_num)
{
	sem_post(philo->end_lock);
	if (!philo->end)
		printf("%ld philo %d has died\n", time, philo_num + 1);
	return_fork(philo);
	usleep(15000);
	sem_post(philo->print_lock);
	return (1);
}

int	lock_print(t_philos *philo, t_philo_state state)
{
	long	time;
	int		philo_num;

	philo_num = philo->philo;
	sem_wait(philo->print_lock);
	if (philo->end)
	{
		sem_post(philo->print_lock);
		return (1);
	}
	time = get_time_since_start();
	if (state != DIED)
		print_action(state, time, philo_num);
	else
		return (print_dead(philo, time, philo_num));
	sem_post(philo->print_lock);
	return (0);
}
