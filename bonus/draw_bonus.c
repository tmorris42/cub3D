/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 20:56:45 by tmorris          ###   ########.fr       */
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

int			draw_hud(t_screen *screen)
{
	int		height;
	t_pt	start;
	t_pt	stop;

	height = screen->height;
	start.x = screen->width - 250;
	stop.x = start.x + 200;
	start.y = height - 40;
	stop.y = start.y + 20;
	ft_draw_rect(screen, screen->height - 55, 50, DARKGRAY);
	draw_rect(screen, start, stop, RED);
	stop.x = start.x + screen->player->hp * 10;
	draw_rect(screen, start, stop, GREEN);
	return (0);
}

void		draw_hud_text(t_screen *screen)
{
	int		height;
	char	msg[100];

	height = screen->height;
	int_to_str(&msg[0], screen->score, 0);
	mlx_string_put(screen->mlx, screen->win, 10, height - 40, GREEN, "Score:");
	mlx_string_put(screen->mlx, screen->win, 15, height - 30, GREEN, msg);
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
	draw_hud(screen);
	mlx_put_image_to_window(screen->mlx, screen->win, screen->buf.img, 0, 0);
	draw_hud_text(screen);
	return (1);
}

int			ft_redraw(t_screen *screen)
{
	screen->refresh = 1;
	return (ft_draw(screen));
}
