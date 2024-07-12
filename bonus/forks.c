/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 11:50:50 by samoore           #+#    #+#             */
/*   Updated: 2024/07/12 14:32:39 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	return_fork(t_philos *philo)
{
	int got_fork;

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
	if (!got_fork)
		return ;
	sem_post(philo->forks);
	if (got_fork == 1)
		return ;
	sem_post(philo->forks);
}

int	take_fork(t_philos *philo)
{
	if (philo->end)
		return (1);
	sem_wait(philo->forks);
	if (philo->end)
	{
		return_fork(philo);
		return (1);
	}
	if (!philo->has_first_fork)
		philo->has_first_fork = 1;
	else
		philo->has_second_fork = 1;
	return (1);
}

