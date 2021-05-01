/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 18:00:38 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			ft_error(char *msg)
{
	ft_putstr_fd("Error\n", ERROR_FD);
	ft_putstr_fd(msg, ERROR_FD);
	ft_putstr_fd("\n", ERROR_FD);
	return (-1);
}

void		ft_close_on_error(t_screen *screen, char *msg)
{
	ft_error(msg);
	ft_close_screen(&screen);
}

t_map_data	*ft_free_map_error(t_map_data *map, char *msg)
{
	ft_error(msg);
	return (ft_free_map_data(map));
}

void		ft_free_map_exit(t_screen *screen, t_map_data *map, char *msg)
{
	ft_free_map_data(map);
	ft_close_on_error(screen, msg);
}
