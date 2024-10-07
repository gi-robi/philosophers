/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiambon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:10:12 by rgiambon          #+#    #+#             */
/*   Updated: 2024/10/07 12:28:10 by rgiambon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		th;
	pthread_t_mutex	fork;
	int				num;
	struct timeval	start;
	struct timeval	end;
}	t_philo;

typedef struct s_data
{
	t_philo			*philo;
	pthread_t		monitor;
	pthread_t_mutex	is_dead;
	pthread_t_mutex	time;
	pthread_t_mutex	printer;
	int				*index;
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_of_meals;
	int				dead_philo;
	struct timeval	start_of_sim;
}	t_data;

int		arg_check(int argc, char **argv);
int		init_values(char **argv, t_data *data);
int		make_threads(t_data *data);
void	routine(t_data *data);
#endif
