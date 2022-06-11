/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 17:10:28 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "raycast.h"

void	ft_draw_col(t_screen *scr, t_sprite_data *data, t_pt *text, t_pt *pixel)
{
	int		color;
	int		d;

	pixel->y = data->draw.start.y;
	while (pixel->y < data->draw.end.y)
	{
		d = pixel->y * 2 - scr->height + data->dim.y;
		text->y = ((d * scr->sprite.height) / data->dim.y) / 2;
		color = ft_get_pixel_from_image(&(scr->sprite), text->x, text->y);
		if ((color & 0x00FFFFFF) != 0)
			ft_pixel_put(&(scr->buf), pixel->x, pixel->y, color);
		++pixel->y;
	}
}

void	ft_render_sprite(t_screen *scr, t_sprite_data *data)
{
	t_pt	pixel;
	t_pt	text;

	pixel.x = data->draw.start.x;
	while (pixel.x < data->draw.end.x)
	{
		text.x = ((pixel.x - (-data->dim.x / 2 + data->screen_x)) *
					scr->sprite.width / data->dim.x);
		if (pixel.x > 0 && pixel.x < scr->width &&
			data->transform.y > 0 && data->transform.y < data->buffer[pixel.x])
		{
			ft_draw_col(scr, data, &text, &pixel);
		}
		++pixel.x;
	}
}

void	ft_draw_sprite(t_screen *screen, t_sprite_data *data, int *i)
{
	double	temp;

	data->pos.x = screen->sprites[data->order[*i]].x;
	data->pos.x -= screen->player->pos_x;
	data->pos.y = screen->sprites[data->order[*i]].y;
	data->pos.y -= screen->player->pos_y;
	ft_calc_transform(screen, data);
	temp = ((screen->width / 2) * (1 + data->transform.x / data->transform.y));
	data->screen_x = (int)temp;
	data->dim.y = (int)(fabs(screen->height / data->transform.y));
	data->draw.start.y = -data->dim.y / 2 + screen->height / 2;
	data->draw.start.y = ft_max(data->draw.start.y, 0);
	data->draw.end.y = data->dim.y / 2 + screen->height / 2;
	data->draw.end.y = ft_min(data->draw.end.y, screen->height);
	data->dim.x = (int)(fabs(screen->height / data->transform.y));
	data->draw.start.x = (int)(-data->dim.x / 2 + data->screen_x);
	data->draw.start.x = ft_max(data->draw.start.x, 0);
	data->draw.end.x = (int)(data->dim.x / 2 + data->screen_x);
	data->draw.end.x = ft_min(data->draw.end.x, screen->width);
	ft_render_sprite(screen, data);
	*i += 1;
}

int		ft_cast_sprites(t_screen *scr, t_sprite_data *data)
{
	int		i;

	if (!(data->order = (int*)malloc(sizeof(*data->order) * scr->sprite_count)))
		return (-1);
	if (!(data->dist = (double*)malloc(sizeof(double) * scr->sprite_count)))
		return (-1);
	i = 0;
	while (i < scr->sprite_count)
	{
		data->order[i] = i;
		data->dist[i] = ((scr->player->pos_x - scr->sprites[i].x) *
				(scr->player->pos_x - scr->sprites[i].x) +
				(scr->player->pos_y - scr->sprites[i].y) *
				(scr->player->pos_y - scr->sprites[i].y));
		++i;
	}
	ft_sort_sprites(data, scr->sprite_count);
	i = 0;
	while (i < scr->sprite_count)
		ft_draw_sprite(scr, data, &i);
	return (0);
}
