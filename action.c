/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorisak <hmorisak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 21:03:16 by hikaru            #+#    #+#             */
/*   Updated: 2023/06/24 19:16:57 by hmorisak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_dead_flag(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->eat);
	if (philo->data->die_flag == TRUE)
	{
		pthread_mutex_unlock(&philo->data->eat);
		return (TRUE);
	}
	pthread_mutex_unlock(&philo->data->eat);
	return (FALSE);
}

void	take_fork(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->fork[philo->right_fork]);
	if (check_dead_flag(philo) == FALSE)
		print_msg(philo, FORK_MSG, get_time() - data->start_time);
	pthread_mutex_lock(&data->fork[philo->left_fork]);
	if (check_dead_flag(philo) == FALSE)
		print_msg(philo, FORK_MSG, get_time() - data->start_time);
}

void	only_one(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->fork[philo->right_fork]);
	print_msg(philo, FORK_MSG, get_time() - data->start_time);
	while (1)
	{
		usleep(data->time_to_die);
		pthread_mutex_lock(&data->eat);
		if (data->die_flag == TRUE)
			break ;
		pthread_mutex_unlock(&data->eat);
	}
	pthread_mutex_unlock(&data->eat);
	pthread_mutex_unlock(&data->fork[philo->right_fork]);
}

void	eating(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->philo_num == 1)
	{
		only_one(philo, data);
		return ;
	}
	take_fork(philo, data);
	if (check_dead_flag(philo) == FALSE)
		print_msg(philo, EAT_MSG, get_time() - data->start_time);
	pthread_mutex_lock(&data->eat);
	philo->next_eat_time = get_time() + data->time_to_die;
	philo->eat_num++;
	pthread_mutex_unlock(&data->eat);
	sleeping(data->time_to_eat, data);
	pthread_mutex_unlock(&data->fork[philo->right_fork]);
	pthread_mutex_unlock(&data->fork[philo->left_fork]);
}

void	sleeping(unsigned long time, t_data *data)
{
	unsigned long	start;

	start = get_time();
	while (get_time() - start < time)
	{
		usleep(time);
		pthread_mutex_lock(&data->eat);
		if (data->die_flag == TRUE)
		{
			pthread_mutex_unlock(&data->eat);
			return ;
		}
		pthread_mutex_unlock(&data->eat);
	}
}
