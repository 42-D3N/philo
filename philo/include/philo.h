/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-pape <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 11:32:29 by tle-pape          #+#    #+#             */
/*   Updated: 2025/03/14 12:41:00 by tle-pape         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# define HOW "Must have 4 or 5 arguments.\nPhilosopher need to be used like"
# define TO " this :\n./philo [num. of philos] [time before death] [time t"
# define U "o eat] [time to sleep] [num. of philo need to eat]\n"
# define S "Last arg is optional, if there is no last arg, the program wi"
# define E "ll run indefinitly.\n"
# define FORK "has taken a fork."
# define EAT "is eating..."
# define SLEEP "is sleeping..."
# define THINK "is thinking..."
# define DEAD "died."

typedef struct s_data
{
	int				error;
	int				philo_num;
	int				ded;
	int				nom;
	int				zzz;
	int				must_eat;
	int				stop_sim;
	long int		start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	safeprint;
	pthread_mutex_t	dead_lock;
	pthread_t		dead_checker;
}				t_data;

typedef struct s_philo
{
	int				num;
	int				ate;
	long int		last_eat;
	t_data			*data;
	pthread_t		thread;
	pthread_mutex_t	mealock;
}				t_philo;

int			ft_atoi(char *str);
char		*is_digit_only(char *str);
long int	gettimestamp(t_philo *data);
void		s_printf(t_philo *philo, char *msg);
void		p_sleep(long int waittime);
void		clear_data(t_data *data, t_philo *philos);
void		*philo_life(void *vargp);
void		*check_death(void *vargp);

#endif
