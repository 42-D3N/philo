/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-pape <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:30:56 by tle-pape          #+#    #+#             */
/*   Updated: 2025/03/14 12:45:05 by tle-pape         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static void	thread_creation(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	data->start = gettimestamp(NULL);
	data->stop_sim = 0;
	pthread_create(&data->dead_checker, NULL, check_death, philos);
	while (i < data->philo_num)
	{
		pthread_create(&philos[i].thread, NULL, philo_life, &philos[i]);
		i++;
	}
}

static void	philos_init(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_init(&philos[i].mealock, NULL);
		philos[i].num = i + 1;
		philos[i].ate = 0;
		philos[i].last_eat = gettimestamp(NULL);
		philos[i].data = data;
		i++;
	}
}

static void	mutex_init(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			data->error = -2;
		i++;
	}
	if (pthread_mutex_init(&data->safeprint, NULL) != 0)
		data->error = -2;
	if (pthread_mutex_init(&data->dead_lock, NULL) != 0)
		data->error = -2;
}

static void	philo_setup(t_data *data, int argc, char **argv)
{
	data->error = 0;
	data->philo_num = ft_atoi(is_digit_only(argv[1]));
	data->ded = ft_atoi(is_digit_only(argv[2]));
	data->nom = ft_atoi(is_digit_only(argv[3]));
	data->zzz = ft_atoi(is_digit_only(argv[4]));
	if (argc == 6)
		data->must_eat = ft_atoi(is_digit_only(argv[5]));
	else
		data->must_eat = 2147483647;
	if (data->ded < 1 || data->nom < 1 || data->zzz < 1 || data->must_eat < 1
		|| data->philo_num > 200 || data->philo_num < 1)
		data->error = -2;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->forks)
		data->error = -2;
	mutex_init(data);
}

int	main(int argc, char **argv)
{
	int		i;
	t_data	data;
	t_philo	*philos;

	i = 0;
	if (argc != 5 && argc != 6)
		return (printf(HOW TO U S E));
	philo_setup(&data, argc, argv);
	philos = malloc(sizeof(t_philo) * data.philo_num);
	if (data.error < 0 || !philos)
	{
		printf("Error during setup\n");
		free(philos);
		clear_data(&data, NULL);
		return (0);
	}
	philos_init(&data, philos);
	thread_creation(&data, philos);
	while (i < data.philo_num)
		pthread_join(philos[i++].thread, NULL);
	pthread_join(data.dead_checker, NULL);
	clear_data(&data, philos);
}
