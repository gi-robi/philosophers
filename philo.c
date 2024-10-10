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

int	pick_up_forks(t_data *data, int philo_num)
{
	int	right;
	int	left;

	right = (data->index + 1) % data->philo_num;
	left = data->index;
	if (left < right)
	{
		pthread_mutex_lock(&data->philo[left].fork);
		if (printer("has taken a fork", data, philo_num) != 0)
			return (1);
		pthread_mutex_lock(&data->philo[right].fork);
		if (printer("has taken a fork", data, philo_num) != 0)
			return (1);
	}
	else
	{
		pthread_mutex_lock(&data->philo[right].fork);
		if (printer("has taken a fork", data, philo_num) != 0)
			return (1);
		pthread_mutex_lock(&data->philo[left].fork);
		if (printer("has taken a fork", data, philo_num) != 0)
			return (1);
	}
	return (0);
}

void	put_down_forks(t_data *data)
{
	int	right;
	int	left;

	right = (data->index + 1) % data->philo_num;
	left = data->index;
	pthread_mutex_unlock(&data->philo[left].fork);
	pthread_mutex_unlock(&data->philo[right].fork);
}

void	*monitor_routine(void *arg)
{
	int	starving_time;
	int	i;
	t_data *data;
	
	data = (t_data *)arg;
	while (1)
	{	
		i = 0;
		while (i < data->philo_num)
		{	
			starving_time = get_time_diff(&data->philo[i].last_meal_time);
			if (starving_time > data->time_to_die)
			{
				pthread_mutex_lock(&data->is_dead);
				data->dead_philo = i + 1;
				pthread_mutex_unlock(&data->is_dead);
				return (NULL);
			}
			i++;
		}
	}
	return (NULL);
}

int	eat_and_sleep(t_data *data, int time_to)
{
	int	i;

	i = 0;
	while (i < time_to)
	{
		usleep(1000);
		pthread_mutex_lock(&data->is_dead);
		if (data->dead_philo != 0)
		{
			pthread_mutex_unlock(&data->is_dead);
			return (1);
		}
		pthread_mutex_unlock(&data->is_dead);
		i++;
	}
	return (0);
}
int	all_are_done(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (data->philo[i].meals_eaten < data->n_of_meals)
			return (0);
		i++;
	}
	return (1);
}

int	printer(char *s, t_data *data, int philo_num)
{
	pthread_mutex_lock(&data->is_dead);
	if (data->dead_philo != 0 || all_are_done(data))
	{
		pthread_mutex_unlock(&data->is_dead);
		return (1);
	}
	pthread_mutex_unlock(&data->is_dead);
	pthread_mutex_lock(&data->time);
	printf("%lu %d %s\n", get_time_diff(&data->start_of_sim), philo_num, s);
	pthread_mutex_unlock(&data->time);
	return (0);
}
int	philo_eats(t_data *data, int philo_num)
{
	if (printer("is eating", data, philo_num) != 0)
		return (1);
	pthread_mutex_lock(&data->time);
	gettimeofday(&data->philo[philo_num].last_meal_time, NULL);
	pthread_mutex_unlock(&data->time);
	data->philo[data->index].meals_eaten += 1;
	if (eat_and_sleep(data, data->time_to_eat) != 0)
		return (1);
	return (0);
}
	
void	*routine(void  *arg)
{
	int	philo_num;
	t_data *data; 

	data = (t_data *)arg;
	philo_num = data->philo[data->index].num;
	pthread_mutex_unlock(&data->philo_n);
    printf("[%d] philo fork -> [%p]\n", philo_num, &data->philo[philo_num].fork);
	printf("philo_num in routine = %d\n", philo_num);
	if (pthread_create(&data->monitor, NULL, monitor_routine, data) != 0)
		return (NULL);
	while (1)
	{
		if (printer("is thinking", data, philo_num) != 0)
			return (NULL);
		if (pick_up_forks(data, philo_num) != 0)
			return (NULL);
		if (philo_eats(data, philo_num) != 0)
			return (NULL);
		put_down_forks(data);
		if (printer("is sleeping", data, philo_num) != 0)
			return (NULL);
		eat_and_sleep(data, data->time_to_sleep);
	}
}

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (result * sign);
}

void	init_values(int	argc, char **argv, t_data *data)
{
	int	i;

	data->philo_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->n_of_meals = ft_atoi(argv[5]);
	data->dead_philo = 0;
	data->philo = malloc(data->philo_num * sizeof(t_data));
	i = 0;
	while (i < data->philo_num)
	{
		data->philo[i].num = i + 1;
		data->philo[i].meals_eaten = 0;
		i++;
	}
}

int	make_threads(t_data *data)
{
	int	i;

	i = 0;
	pthread_create(&data->monitor, NULL, monitor_routine, &data);
	while (i < data->philo_num)
	{
		pthread_mutex_lock(&data->philo_n);
		data->index = i;
		printf("philo num in make_threads = %d\n", i);
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
	pthread_join(data->monitor, NULL);
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
	if (pthread_mutex_init(&data->philo_n, NULL) != 0)
		return (0);
	return (1);
}

void	destroy_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->philo[i].fork);
		i++;
	}
	pthread_mutex_destroy(&data->is_dead);
	pthread_mutex_destroy(&data->philo_n);
	pthread_mutex_destroy(&data->time);
}
void	init_time(t_data *data)
{
	int	i;

	i = 0;
	gettimeofday(&data->start_of_sim, NULL);
	while (i < data->philo_num)
	{
		data->philo[i].last_meal_time.tv_sec = data->start_of_sim.tv_sec;
		data->philo[i].last_meal_time.tv_usec = data->start_of_sim.tv_usec;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!arg_check(argc, argv))
		return (0);
	init_values(argc, argv, &data);
	if (!init_mutex(&data))
		return (0);
	init_time(&data);
	if (!make_threads(&data))
		return (0);
	if (data.dead_philo != 0)
		printf("%lu %d is dead\n", get_time_diff(&data.start_of_sim), data.dead_philo);
	destroy_all(&data);
	return (0);
}
