/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 23:07:50 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		ft_error(char *msg)
{
	if (ft_strncmp(msg, "Error\n", 6))
		ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
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
