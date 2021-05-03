/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 20:50:03 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	ft_rotate(double rad, t_screen *screen)
{
	double		old_rx;
	t_player	*p;

	rad = rad / 2;
	p = screen->player;
	old_rx = p->rot_x;
	p->rot_x = (cos(rad) * old_rx) - (p->rot_y * sin(rad));
	p->rot_y = (sin(rad) * old_rx) + (cos(rad) * p->rot_y);
}

void	ft_move_relative(double x, double y, t_screen *screen)
{
	t_player	*p;

	x = x / 10;
	y = y / 10;
	p = screen->player;
	if (screen->map[(int)(p->pos_y)][(int)(p->pos_x + x)] == 0)
		p->pos_x += x;
	else
		screen->player->hp -= 1;
	if (screen->map[(int)(p->pos_y + y)][(int)p->pos_x] == 0)
		p->pos_y += y;
	else
		screen->player->hp -= 1;
	if (screen->player->hp < 1)
	{
		ft_putstr("GAME OVER!");
		ft_close_screen(&screen);
	}
}

int		ft_parse_keys(int key, t_screen *screen, int pressed)
{
	if (key == K_ESCAPE)
		ft_close_screen(&screen);
	else if (key == K_UP || key == K_W)
		screen->keys.up = pressed;
	else if (key == K_A)
		screen->keys.left = pressed;
	else if (key == K_DOWN || key == K_S)
		screen->keys.down = pressed;
	else if (key == K_D)
		screen->keys.right = pressed;
	else if (key == K_RIGHT)
		screen->keys.turn_right = pressed;
	else if (key == K_LEFT)
		screen->keys.turn_left = pressed;
	else
		return (1);
	screen->refresh = 1;
	return (1);
}

int		ft_press_keys(int key, t_screen *screen)
{
	return (ft_parse_keys(key, screen, TRUE));
}

int		ft_lift_keys(int key, t_screen *screen)
{
	return (ft_parse_keys(key, screen, FALSE));
}
