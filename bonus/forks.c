/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:50:50 by samoore           #+#    #+#             */
/*   Updated: 2024/07/08 21:32:31 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	return_fork(t_philos *philo)
{
	int	got_fork;

	got_fork = 0;
	if (philo->has_first_fork)
	{
		got_fork = 1;
		philo->has_first_fork = 0;
	}
	if (philo->has_second_fork)
	{
		got_fork = 2;
		philo->has_second_fork = 0;
	}
	sem_post(philo->forks);
	if (got_fork == 1)
		return ;
	sem_post(philo->forks);
}

int	take_fork(t_philos *philo)
{
	sem_wait(philo->forks);
	if (!philo->has_first_fork)
		philo->has_first_fork = 1;
	else
		philo->has_second_fork = 1;
	return (1);
}
