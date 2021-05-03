/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 20:22:50 by tmorris          ###   ########.fr       */
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

int			draw_hud(t_screen *screen)
{
	char	msg[100];
	int		height;

	height = screen->height;
	ft_draw_rect(screen, screen->height - 55, 50, DARKGRAY);
	int_to_str(&msg[0], screen->score, 0);
	mlx_string_put(screen->mlx, screen->win, 10, height - 40, GREEN, "Score:");
	mlx_string_put(screen->mlx, screen->win, 15, height - 30, GREEN, msg);
	return (0);
}

int			ft_draw(t_screen *screen)
{
	if (screen->refresh == 0)
		return (0);
	screen->refresh = 0;
	ft_draw_rect(screen, 0, screen->height / 2, screen->colors.ceiling);
	ft_draw_rect(screen, screen->height / 2, 1 + screen->height / 2,
			screen->colors.floor);
	if (ft_raycast(screen) == -1)
		return (-1);
	if (!(screen->win))
		return (-1);
	mlx_put_image_to_window(screen->mlx, screen->win, screen->buf.img, 0, 0);
	draw_hud(screen);
	return (1);
}

int			ft_redraw(t_screen *screen)
{
	screen->refresh = 1;
	return (ft_draw(screen));
}
