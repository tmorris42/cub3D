/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/02/26 14:50:49 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	ft_draw_rect(t_screen *screen, int starty, int leny, int color)
{
	int		i;
	int		j;

	j = starty;
	while (j < starty + leny)
	{
		i = 0;
		while (i < screen->width)
		{
			ft_pixel_put(&screen->buf, i, j, color);
			++i;
		}
		++j;
	}
}

int			ft_draw(t_screen *screen)
{
	if (screen->refresh == 0)
		return (0);
	screen->refresh = 0;
	ft_draw_rect(screen, 0, screen->height, screen->colors.ceiling);
	ft_draw_rect(screen, screen->height / 2, screen->height / 2,
			screen->colors.floor);
	if (ft_raycast(screen) == -1)
		return (-1);
	if (!(screen->win))
		return (-1);
	mlx_put_image_to_window(screen->mlx, screen->win, screen->buf.img, 0, 0);
	return (1);
}

int			ft_redraw(t_screen *screen)
{
	screen->refresh = 1;
	return (ft_draw(screen));
}
