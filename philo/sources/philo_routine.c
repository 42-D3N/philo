/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-pape <tle-pape@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:38:02 by tle-pape          #+#    #+#             */
/*   Updated: 2025/03/14 12:49:41 by tle-pape         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	p_sleep(long int waittime)
{
	long int	start;

	start = gettimestamp(NULL);
	while (gettimestamp(NULL) - start < waittime)
		usleep(50);
}

static void	release_fork(t_philo *philo)
{
	pthread_mutex_unlock(&philo->data->forks[(philo->num - 1) \
	% philo->data->philo_num]);
	pthread_mutex_unlock(&philo->data->forks[philo->num \
	% philo->data->philo_num]);
}

static void	philo_eat(t_philo *philo)
{
	if (philo->num % 2 == 0)
	{
		pthread_mutex_lock(&philo->data->forks[philo->num % \
		philo->data->philo_num]);
		s_printf(philo, FORK);
		pthread_mutex_lock(&philo->data->forks[(philo->num - 1) \
		% philo->data->philo_num]);
		s_printf(philo, FORK);
	}
	else
	{
		usleep(1000);
		pthread_mutex_lock(&philo->data->forks[(philo->num - 1) \
		% philo->data->philo_num]);
		s_printf(philo, FORK);
		pthread_mutex_lock(&philo->data->forks[philo->num % \
		philo->data->philo_num]);
		s_printf(philo, FORK);
	}
	pthread_mutex_lock(&philo->mealock);
	philo->last_eat = gettimestamp(NULL);
	philo->ate++;
	pthread_mutex_unlock(&philo->mealock);
	s_printf(philo, EAT);
	p_sleep(philo->data->nom);
}

static void	philo_loop(t_philo *philo)
{
	while (1)
	{
		pthread_mutex_lock(&philo->data->dead_lock);
		if (philo->data->stop_sim)
		{
			pthread_mutex_unlock(&philo->data->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->data->dead_lock);
		philo_eat(philo);
		pthread_mutex_lock(&philo->data->dead_lock);
		if (philo->data->must_eat > 0 && philo->ate >= philo->data->must_eat)
		{
			pthread_mutex_unlock(&philo->data->dead_lock);
			release_fork(philo);
			return ;
		}
		pthread_mutex_unlock(&philo->data->dead_lock);
		release_fork(philo);
		s_printf(philo, SLEEP);
		p_sleep(philo->data->zzz);
		s_printf(philo, THINK);
	}
}

void	*philo_life(void *vargp)
{
	t_philo	*philo;

	philo = (t_philo *)vargp;
	if (philo->data->philo_num == 1)
	{
		s_printf(philo, FORK);
		pthread_mutex_lock(&philo->data->dead_lock);
		p_sleep(philo->data->ded);
		pthread_mutex_unlock(&philo->data->dead_lock);
		return (NULL);
	}
	if (philo->num % 2 == 0)
		usleep(100);
	philo_loop(philo);
	return (NULL);
}
