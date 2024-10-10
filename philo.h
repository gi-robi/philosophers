#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	pthread_t		th;
	pthread_mutex_t		fork;
	int				num;
	struct timeval		last_meal_time;
	int		meals_eaten;
}	t_philo;

typedef struct s_data
{
	t_philo			*philo;
	pthread_t		monitor;
	pthread_mutex_t philo_n;
	pthread_mutex_t	is_dead;
	pthread_mutex_t	time;
	pthread_mutex_t	printer;
	int				index;
	int				philo_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_of_meals;
	int				dead_philo;
	struct timeval	start_of_sim;
}	t_data;

int		arg_check(int argc, char **argv);
void		init_values(int argc, char **argv, t_data *data);
int		make_threads(t_data *data);
void	*routine(void *arg);
int     printer(char *s, t_data *data, int philo_num);
#endif
