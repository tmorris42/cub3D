/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 17:10:28 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "raycast.h"

void	ft_sort_sprites(t_sprite_data *sprite_data, int amount)
{
	int		i;
	int		hold_order;
	double	hold_dist;

	i = 0;
	while (i < amount - 1)
	{
		if (sprite_data->dist[i] < sprite_data->dist[i + 1])
		{
			hold_order = sprite_data->order[i + 1];
			hold_dist = sprite_data->dist[i + 1];
			sprite_data->order[i + 1] = sprite_data->order[i];
			sprite_data->order[i] = hold_order;
			sprite_data->dist[i + 1] = sprite_data->dist[i];
			sprite_data->dist[i] = hold_dist;
			if (i > 0)
				--i;
		}
		else
			++i;
	}
}

void	ft_calc_transform(t_screen *screen, t_sprite_data *data)
{
	double	total;
	double	inv_det;

	inv_det = (data->plane.x * screen->player->rot_y);
	inv_det -= (screen->player->rot_x * data->plane.y);
	inv_det = 1.0 / inv_det;
	total = screen->player->rot_y * data->pos.x;
	total -= (screen->player->rot_x * data->pos.y);
	total *= inv_det;
	data->transform.x = total;
	total = (-data->plane.y * data->pos.x);
	total += (data->plane.x * data->pos.y);
	total *= inv_det;
	data->transform.y = total;
}

int		ft_free_sprite_data(t_sprite_data *data)
{
	free(data->order);
	free(data->dist);
	free(data->buffer);
	data->order = NULL;
	data->dist = NULL;
	data->buffer = NULL;
	return (-1);
}

void	ft_init_sprite_data(t_sprite_data *data)
{
	data->dim.x = 0;
	data->dim.y = 0;
	data->pos.x = 0.0;
	data->pos.y = 0.0;
	data->screen_x = 0.0;
	data->buffer = NULL;
	data->order = NULL;
	data->dist = NULL;
	data->plane.x = 0.0;
	data->plane.y = 0.0;
	data->draw.start.x = 0;
	data->draw.start.y = 0;
	data->draw.end.x = 0;
	data->draw.end.y = 0;
	data->transform.x = 0.0;
	data->transform.y = 0.0;
}
