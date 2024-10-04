/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiambon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:10:12 by rgiambon          #+#    #+#             */
/*   Updated: 2024/10/02 17:44:57 by rgiambon         ###   ########.fr       */
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
	pthread_t	th;
	int		num;
	struct timeval	start;
	struct timeval	end;
}	t_philo;

typedef struct s_data
{
	t_philo			*philo;
	pthread_mutex_t	mutex;
	int			philo_num;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
}	t_data;

int		arg_check(int argc, char **argv);
int		init_values(char **argv, t_data *data);
int		make_threads(t_data *data);
void	routine(t_data *data);
#endif
