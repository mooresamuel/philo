/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samoore <samoore@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/06 11:29:16 by samoore           #+#    #+#             */
/*   Updated: 2024/07/06 14:37:23 by samoore          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

int	dead(int num, pthread_mutex_t *dead_lock)
{
	static int	dead = 0;

	pthread_mutex_lock(dead_lock);
	dead += num;
	num = dead;
	pthread_mutex_unlock(dead_lock);
	return (num);
}

int end(int num, pthread_mutex_t *end_lock)
{
	static int	end = 0;

	pthread_mutex_lock(end_lock);
	end += num;
	num = end;
	pthread_mutex_unlock(end_lock);
	return (num);	
}