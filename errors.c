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

void	ft_free_map_and_close(t_screen *screen, t_map_data *map, char *msg)
{
	ft_free_map_data(map);
	ft_close_on_error(screen, msg);
}

void	ft_close_on_error(t_screen *screen, char *msg)
{
	perror(msg);
	ft_close_screen(&screen);
}
