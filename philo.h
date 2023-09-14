/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorisak <hmorisak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 23:28:25 by hikaru            #+#    #+#             */
/*   Updated: 2023/06/24 18:59:03 by hmorisak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <sys/time.h>

# include <string.h>

# define TRUE 0
# define FALSE 1

# define FORK_MSG "has taken a fork"
# define EAT_MSG "is eating"
# define SLEEP_MSG "is sleeping"
# define THINK_MSG "is thinking"
# define DIED "died"

typedef struct s_philo
{
	struct s_data	*data;
	pthread_t		th;
	int				id;
	int				eat_num;
	unsigned long	next_eat_time;
	int				right_fork;
	int				left_fork;
}	t_philo;

typedef struct s_data
{
	int				philo_num;
	unsigned long	start_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	int				max_eat_philo;
	int				die_flag;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	eat;
	pthread_mutex_t	write;
}	t_data;

//action.c
void			take_fork(t_philo *philo, t_data *data);
void			only_one(t_philo *philo, t_data *data);
void			eating(t_philo *philo);
int				check_dead_flag(t_philo *philo);
void			sleeping(unsigned long time, t_data *data);

//ft_atoi.c
int				ft_atoi(const char *str);
int				is_int(char *str); //数字以外、マイナス、オーバーフローはFALSE

//ft_init.c
int				init_mutex(t_data *data);
void			init_philo(t_data *data);
int				init_data(t_data *data);

//main.c
unsigned long	get_time(void);
int				check_param(int argc, char **argv, t_data *data);

//philo.c
void			print_msg(t_philo *philo, char *str, unsigned long time);
void			*routine(void *philo_data);
void			check_died(t_data *data);
void			destroy_mutex(t_data *data);
int				philo(t_data *data);

#endif