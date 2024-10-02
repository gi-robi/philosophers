/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgiambon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:09:02 by rgiambon          #+#    #+#             */
/*   Updated: 2024/10/02 17:45:10 by rgiambon         ###   ########.fr       */
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
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

void	routine(t_data *data)
{
}

int	init_values(char **argv, t_data *data)
{
	data->philo_num = ft_atoi(argv[1]);
	data->threads = malloc(data.philo_num * sizeof(pthread_t));
	if (threads == NULL)
		return (0);
	data->time_to_die = argv[2];
	data->time_to_eat = argv[3];
	data->time_to_sleep = argv[4];
	data->forks = data->philo_num;
}

int	make_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_create(&threads[i], NULL, routine, data) != 0)
		{
			free(data->threads);
			return (0);
		}
		i++;
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			free(data->threads);
			return (0);
		}
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (!arg_check(argc, argv))
		return (0);
	if (!init_values(argv, &data))
		return (0);
	if (!make_threads(&data))
		return (0);
	free(data.threads);
}
