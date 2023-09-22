/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorisak <hmorisak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 16:15:59 by hikaru            #+#    #+#             */
/*   Updated: 2023/06/24 19:14:40 by hmorisak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_msg(t_philo *philo, char *str, unsigned long time)
{
	pthread_mutex_lock(&philo->data->write);
	pthread_mutex_lock(&philo->data->eat);
	if (philo->data->die_flag == TRUE && strcmp(str, DIED) != 0)
	{
		pthread_mutex_unlock(&philo->data->eat);
		pthread_mutex_unlock(&philo->data->write);
		return ;
	}
	pthread_mutex_unlock(&philo->data->eat);
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

void	*routine(void *philo_data)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)philo_data;
	data = philo->data;
	while (get_time() < data->start_time)
		usleep(100);
	if (philo->id % 2 == 1 && data->philo_num > 1)
		usleep(data->time_to_eat * 100);
	while (1)
	{
		pthread_mutex_lock(&data->eat);
		if (philo->data->die_flag == TRUE
			|| philo->eat_num >= data->num_must_eat)
			break ;
		pthread_mutex_unlock(&data->eat);
		eating(philo);
		if (check_dead_flag(philo) == FALSE)
			print_msg(philo, SLEEP_MSG, get_time() - data->start_time);
		sleeping(data->time_to_sleep, data);
		if (check_dead_flag(philo) == FALSE)
			print_msg(philo, THINK_MSG, get_time() - data->start_time);
	}
	pthread_mutex_unlock(&data->eat);
	return (NULL);
}

void	check_died(t_data *data)
{
	int	i;

	i = 0;
	while (1)
	{
		i = 0;
		data->max_eat_philo = 0;
		while (i < data->philo_num && data->die_flag == FALSE)
		{
			pthread_mutex_lock(&data->eat);
			if (get_time() >= (unsigned long)data->philo[i].next_eat_time)
			{
				data->die_flag = TRUE;
				pthread_mutex_unlock(&data->eat);
				print_msg(&data->philo[i], DIED, get_time() - data->start_time);
				return ;
			}
			if (data->philo[i].eat_num >= data->num_must_eat)
				data->max_eat_philo++;
			pthread_mutex_unlock(&data->eat);
			i++;
		}
		if (data->max_eat_philo == data->philo_num)
			return ;
	}
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->fork[i]);
		i++;
	}
	pthread_mutex_destroy(&data->eat);
	pthread_mutex_destroy(&data->write);
	free(data->philo);
	free(data->fork);
}

int	philo(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time() + 10;
	while (i < data->philo_num)
	{
		data->philo[i].next_eat_time += data->start_time;
		if (pthread_create(&data->philo[i].th, NULL,
				&routine, &data->philo[i]) != 0)
			return (FALSE);
		i++;
	}
	check_died(data);
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_join(data->philo[i].th, NULL) != 0)
			return (FALSE);
		i++;
	}
	destroy_mutex(data);
	return (TRUE);
}
