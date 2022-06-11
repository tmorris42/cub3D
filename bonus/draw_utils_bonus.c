/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 20:56:55 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		draw_rect(t_screen *screen, t_pt start, t_pt stop, int color)
{
	int		i;
	int		j;

	j = start.y;
	while (j < stop.y)
	{
		i = start.x;
		while (i < stop.x)
		{
			ft_pixel_put(&screen->buf, i, j, color);
			++i;
		}
		++j;
	}
}

int			int_to_str(char *msg, int x, int i)
{
	if (x < 10)
	{
		msg[i] = x + '0';
		msg[i + 1] = '\0';
	}
	else
	{
		i = int_to_str(msg, x / 10, i);
		i = int_to_str(msg, x % 10, i);
	}
	return (i + 1);
}
