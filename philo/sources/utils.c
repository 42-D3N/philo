/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-pape <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 09:40:58 by tle-pape          #+#    #+#             */
/*   Updated: 2025/03/14 12:41:56 by tle-pape         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	clear_data(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	if (philos)
	{
		while (i < data->philo_num)
		{
			pthread_mutex_destroy(&philos[i].mealock);
			i++;
		}
		free(philos);
	}
	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->safeprint);
	pthread_mutex_destroy(&data->dead_lock);
	free(data->forks);
}

void	s_printf(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->data->dead_lock);
	if (!philo->data->stop_sim || msg[0] == 'd')
	{
		pthread_mutex_lock(&philo->data->safeprint);
		printf("%ld\t%d\t%s\n", gettimestamp(philo), philo->num, msg);
		pthread_mutex_unlock(&philo->data->safeprint);
	}
	pthread_mutex_unlock(&philo->data->dead_lock);
}

long int	gettimestamp(t_philo *philo)
{
	struct timeval	comp;

	gettimeofday(&comp, NULL);
	if (!philo)
		return (comp.tv_sec * 1000 + comp.tv_usec / 1000);
	return ((comp.tv_sec * 1000 + comp.tv_usec / 1000) - philo->data->start);
}

char	*is_digit_only(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return ("0");
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			i++;
		else
			return ("0");
	}
	return (str);
}

int	ft_atoi(char *str)
{
	int	res;
	int	sign;

	res = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == 45 || *str == 43)
	{
		if (*str == 45)
			sign = -1;
		str++;
	}
	while (*str >= 48 && *str <= 57)
	{
		res = res * 10;
		res = res + *str - 48;
		str++;
	}
	res *= sign;
	if (res < 0 || (res > 0 && sign == -1))
		res = -1;
	return (res);
}
