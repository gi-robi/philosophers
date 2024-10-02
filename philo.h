#ifndef
# define

typedef struct s_data
{
	pthread_t	*threads;
	int		philo_num;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		forks;
}	t_data;


#endif
