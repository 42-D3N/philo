/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-pape <tle-pape@student.42lehavre.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:37:51 by tle-pape          #+#    #+#             */
/*   Updated: 2025/03/14 10:34:18 by tle-pape         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	philo_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->mealock);
	if (gettimestamp(NULL) - philo->last_eat > philo->data->ded)
	{
		pthread_mutex_lock(&philo->data->dead_lock);
		philo->data->stop_sim = 1;
		pthread_mutex_unlock(&philo->data->dead_lock);
		s_printf(philo, DEAD);
		pthread_mutex_unlock(&philo->mealock);
		return (1);
	}
	pthread_mutex_unlock(&philo->mealock);
	return (0);
}

static int	enough_meal(t_philo *philos, t_data *data)
{
	int	i;
	int	meals;

	i = 0;
	meals = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_lock(&philos[i].mealock);
		if (data->must_eat > 0 && philos[i].ate >= data->must_eat)
			meals++;
		pthread_mutex_unlock(&philos[i].mealock);
		i++;
	}
	return (meals == data->philo_num);
}

void	*check_death(void *vargp)
{
	int		i;
	t_data	*data;
	t_philo	*philos;

	philos = (t_philo *)vargp;
	data = philos[0].data;
	while (1)
	{
		i = 0;
		while (i < data->philo_num)
		{
			if (philo_death(&philos[i]))
				return (NULL);
			i++;
		}
		if (enough_meal(philos, data))
			return (NULL);
		usleep(50);
	}
	return (NULL);
}
