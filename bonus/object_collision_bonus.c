/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_collision_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 19:12:19 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void			delete_sprite(t_screen *screen, int sprite_index)
{
	int		i;

	if (sprite_index > screen->sprite_count)
		return ;
	i = 0;
	while (i < sprite_index && i < screen->sprite_count)
		++i;
	while (i < screen->sprite_count - 1)
	{
		screen->sprites[i].x = screen->sprites[i + 1].x;
		screen->sprites[i].y = screen->sprites[i + 1].y;
		++i;
	}
	screen->sprites[i].x = -1;
	screen->sprites[i].y = -1;
	--(screen->sprite_count);
}

void			check_object_collision(t_screen *screen)
{
	int			i;
	t_sprite	*sprite;

	i = 0;
	while (i < screen->sprite_count)
	{
		sprite = &screen->sprites[i];
		if (floor(sprite->x) == floor(screen->player->pos_x) &&
				floor(sprite->y) == floor(screen->player->pos_y))
			delete_sprite(screen, i);
		++i;
	}
}
