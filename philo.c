/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiambon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:09:02 by rgiambon          #+#    #+#             */
/*   Updated: 2024/10/07 12:51:58 by rgiambon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	arg_check(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc < 4)
		return (0);
	i = 1;
	while (argv[i] != NULL)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '1' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

long	get_time_diff(struct timeval *start)
{
	struct timeval	curr_time;
	long			milliseconds;
	long			sec_diff;
	long			microsec_diff;

	gettimeofday(&curr_time, NULL);
	sec_diff = curr_time.tv_sec - start->tv_sec;
	microsec_diff = curr_time.tv_usec - start->tv_usec;
	milliseconds = (sec_diff * 1000) + (microsec_diff / 1000);
	return (milliseconds);
}

void	pick_up_forks(t_data *data, int philo_num)
{
	int	right;
	int	left;

	right = (data->index + 1) % data->philo_num;
	left = data->index;
	if (left < right)
	{
		pthread_mutex_lock(&data->philo[left].mutex);
		printf("%lu %d has taken a fork\n", get_curr_time(&data->start_of_sim), philo_num);
		pthread_mutex_lock(&data->philo[right].mutex);
		printf("%lu %d has taken a fork\n", get_curr_time(&data->start_of_sim), philo_num);
	}
	else
	{
		pthread_mutex_lock(&data->philo[right].mutex);
		printf("%lu %d has taken a fork\n", get_curr_time(&data->start_of_sim), philo_num);
		pthread_mutex_lock(&data->philo[left].mutex);
		printf("%lu %d has taken a fork\n", get_curr_time(&data->start_of_sim), philo_num);
	}
}

void	put_down_forks(t_data *data)
{
	int	right;
	int	left;

	right = (data->index + 1) % data->philo_num;
	left = data->index;
	pthread_mutex_unlock(&data->philo[left].mutex);
	pthread_mutex_unlock(&data->philo[right].mutex);
}

void	monitor_routine(t_data *data)
{
	int	starving_time;
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		starving_time = get_time_diff(data->philo[i].start);
		if (starving_time > data->time_to_die)
		{
			pthread_mutex_lock(&data->is_dead)
			data->dead_philo = i + 1;
			pthread_mutex_unlock(&data->is_dead);
			return ;
		}
		i++;
	}
}

void	sleep_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->time_to_sleep)
	{
		usleep(1000);
		pthread_mutex_lock(&data->is_dead);
		if (&data->is_dead)
		{
			pthread_mutex_unlock(&data->is_dead);
			return ;
		}
		pthread_mutex_unlock(&data->is_dead);
		i++;
	}
}

void	routine(t_data *data)
{
	int	philo_num;
	int	meals_eaten;

	meals_eaten = 0;
	philo_num = data->philo[data->index].num;
//put everything in a loop that finishes when philo dies
//	*time* *x* is thinking
	while (1)
	{
//add print mutex
		printf("%lu %d is thinking\n", get_time_diff(&data->start_of_sim), philo_num);
		pick_up_forks(data, philo_num);
//philo eats	
		printf("%lu %d is eating\n", get_time_diff(&data->start_of_sim), philo_num);
		data->philo[data->index].last_meal_time = get_current_time(data->start_of_sim); 
		meals_eaten++;
		usleep(data->time_to_eat * 1000);
//puts down forks
		put_down_forks(data);
//philo sleeps
		printf("%lu %d is sleeping\n", get_time_diff(&data->start_of_sim), philo_num);
		sleep_philo(data);
	}
}

void	init_values(int	argc, char **argv, t_data *data)
{
	int	i;

	data->philo_num = ft_atoi(argv[1]);
	data->time_to_die = argv[2];
	data->time_to_eat = argv[3];
	data->time_to_sleep = argv[4];
	if (argc == 6)
		data->n_of_meals = argv[5];
	data->dead_philo = -1;
	i = 0;
	while (i < data->philo_num)
	{
		gettimeofday(&data->philo[i].start, NULL);
		data->philo[i].num = i + 1;
		i++;
	}
}

int	make_threads(t_data *data)
{
	int	i;

	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (0);
	i = 0;
	while (i < data->philo_num)
	{
		data->index = i;
		if (pthread_create(&data->philo[i].th, NULL, routine, data) != 0)
			return (0);
		i++;
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->philo[i].th, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->philo[i].fork, NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->is_dead, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->time, NULL) != 0)
		return (0);
	return (1);
}

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthreads_mutex_destroy(&data->philo[i].fork, NULL);
		i++;
	}
	pthreads_mutex_destroy(&data->is_dead, NULL);
	pthreads_mutex_destroy(&data->time, NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (!arg_check(argc, argv))
		return (0);
	init_values(argc, argv, &data);
	if (init_mutex(&data))
		return (0);
	gettimeofday(&data.start_of_sim);
	if (!make_threads(&data))
		return (0);
	destroy_all(&data);
	return (0);
}
