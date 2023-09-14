/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmorisak <hmorisak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 11:54:29 by hikaru            #+#    #+#             */
/*   Updated: 2023/06/24 19:16:43 by hmorisak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->fork[i], NULL))
			return (FALSE);
		i++;
	}
	if (pthread_mutex_init(&data->eat, NULL))
		return (FALSE);
	if (pthread_mutex_init(&data->write, NULL))
		return (FALSE);
	return (TRUE);
}

void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		data->philo[i].data = data;
		data->philo[i].id = i + 1;
		data->philo[i].eat_num = 0;
		data->philo[i].next_eat_time = data->time_to_die;
		data->philo[i].right_fork = i;
		data->philo[i].left_fork = i + 1;
		if (i == data->philo_num - 1)
			data->philo[i].left_fork = 0;
		i++;
	}
}

int	init_data(t_data *data)
{
	data->philo = (t_philo *)malloc(sizeof(t_philo) * data->philo_num);
	if (!data->philo)
		return (FALSE);
	data->fork = malloc(sizeof(pthread_mutex_t) * data->philo_num);
	if (!data->fork)
	{
		free(data->philo);
		return (FALSE);
	}
	if (init_mutex(data) == FALSE)
	{
		free(data->philo);
		free(data->fork);
		return (FALSE);
	}
	init_philo(data);
	return (TRUE);
}
