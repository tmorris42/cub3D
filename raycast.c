/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 18:42:01 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

typedef struct	s_segment
{
	t_pt	start;
	t_pt	end;
}				t_seg;

typedef struct	s_sprite_data
{
	t_pt	dim;
	t_d_pt	pos;
	double	screen_x;
	double	*buffer;
	int		*order;
	double	*dist;
	t_d_pt	plane;
	t_seg	draw;
	t_d_pt	transform;
}				t_sprite_data;

typedef struct	s_camera
{
//	double	x;
	t_d_pt	plane;
	t_d_pt	ray_dir;
	t_pt	map;
	t_pt	step;
	t_d_pt	dist;
	t_d_pt	delta_dist;
}				t_camera;

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
		}
		++i;
	}
}

void	ft_render_sprite(t_screen *scr, t_sprite_data *data)
{
	t_pt	pixel;
	t_pt	text;
	int		color;
	int		d; //what isthis? needs better name

	pixel.x = data->draw.start.x;
	while (pixel.x < data->draw.end.x)
	{
		text.x = (256 *
				(pixel.x - (-data->dim.x / 2 + data->screen_x)) *
				scr->sprite.width / data->dim.x) / 256;
		if (pixel.x > 0 && pixel.x < scr->width &&
			data->transform.y > 0 && data->transform.y < data->buffer[pixel.x])
		{
			pixel.y = data->draw.start.y;
			while (pixel.y < data->draw.end.y)
			{
				d = pixel.y * 256 - scr->height * 128 + data->dim.y * 128;
				text.y = ((d * scr->sprite.height) / data->dim.y) / 256;
				color = ft_get_pixel_from_image(&(scr->sprite), text.x, text.y);
				if ((color & 0x00FFFFFF) != 0)
					ft_pixel_put(&(scr->buf), pixel.x, pixel.y, color);
				++pixel.y;
			}
		}
		++pixel.x;
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
	data->dim.y = (int)(abs(screen->height / data->transform.y));
	data->draw.start.y = -data->dim.y / 2 + screen->height / 2;
	data->draw.start.y = ft_max(data->draw.start.y, 0);
	data->draw.end.y = data->dim.y / 2 + screen->height / 2;
	data->draw.end.y = ft_min(data->draw.end.y, screen->height);
	data->dim.x = (int)(abs(screen->height / data->transform.y));
	data->draw.start.x = (int)(-data->dim.x / 2 + data->screen_x);
	data->draw.start.x = ft_max(data->draw.start.x, 0);
	data->draw.end.x = (int)(data->dim.x / 2 + data->screen_x);
	data->draw.end.x = ft_min(data->draw.end.x, screen->width);
	ft_render_sprite(screen, data);
	*i += 1;
}

int		ft_cast_sprites(t_screen *screen, t_d_pt plane, double *sprite_buffer)
{
	t_sprite_data	sprite_data;
	int		i;

	sprite_data.buffer = sprite_buffer;
	sprite_data.plane = plane;
	if (!(sprite_data.order = (int*)malloc(sizeof(*sprite_data.order) * screen->sprite_count)))
		return (-1);
	if (!(sprite_data.dist = (double*)malloc(sizeof(*sprite_data.dist) * screen->sprite_count)))
	{
		free(sprite_data.order);
		return (-1);
	}
	i = 0;
	while (i < screen->sprite_count)
	{
		sprite_data.order[i] = i;
		sprite_data.dist[i] = ((screen->player->pos_x - screen->sprites[i].x) * (screen->player->pos_x - screen->sprites[i].x) + (screen->player->pos_y - screen->sprites[i].y) * (screen->player->pos_y - screen->sprites[i].y));
		++i;
	}
	ft_sort_sprites(&sprite_data, screen->sprite_count);
	i = 0;
	while (i < screen->sprite_count)
		ft_draw_sprite(screen, &sprite_data, &i);
	printf("done drawing sprites\n");
	free(sprite_data.order);
	free(sprite_data.dist);
	return (0);
}

void	ft_init_camera(t_screen *screen, t_camera *cam, int x)
{
	double	cam_x;

	cam_x = 2 * x / (double)screen->width - 1;
	cam->ray_dir.x = screen->player->rot_x + cam->plane.x * cam_x;
	cam->ray_dir.y = screen->player->rot_y + cam->plane.y * cam_x;
	cam->map.x = (int)(screen->player->pos_x);
	cam->map.y = (int)(screen->player->pos_y);
	if (cam->ray_dir.y == 0)
		cam->delta_dist.x = 0;
	else if (cam->ray_dir.x == 0)
		cam->delta_dist.x = 1;
	else
		cam->delta_dist.x = fabs(1 / cam->ray_dir.x);
	if (cam->ray_dir.x == 0)
		cam->delta_dist.y = 0;
	else if (cam->ray_dir.y == 0)
		cam->delta_dist.y = 1;
	else
		cam->delta_dist.y = fabs(1 / cam->ray_dir.y);
	if (cam->ray_dir.x < 0)
	{
		cam->step.x = -1;
		cam->dist.x = (screen->player->pos_x - cam->map.x) * cam->delta_dist.x;
	}
	else
	{
		cam->step.x = 1;
		cam->dist.x = (cam->map.x + 1.0 - screen->player->pos_x) * cam->delta_dist.x;
	}
	if (cam->ray_dir.y < 0)
	{
		cam->step.y = -1;
		cam->dist.y = (screen->player->pos_y - cam->map.y) * cam->delta_dist.y;
	}
	else
	{
		cam->step.y = 1;
		cam->dist.y = (cam->map.y + 1.0 - screen->player->pos_y) * cam->delta_dist.y;
	}
}

int		ft_raycast(t_screen *screen)
{
	int		x;
	t_camera	cam;
	t_d_pt	ray_dir;
	t_d_pt	delta_dist;
	t_pt	map;
	t_d_pt	dist; //side_dist
	t_pt	step;
	double	wall_dist;
	int		hit;
	int		side_check;
	double	sprite_buffer[screen->width]; //must be malloc'd instead

	cam.plane.x = -.50 * screen->player->rot_y;
	cam.plane.y = (-.50 * (-screen->player->rot_x));
	printf("<%f, %f>\n", cam.plane.x, cam.plane.y);
	x = 0;
	printf("START LOOP with player location = < %f, %f >\n", screen->player->pos_x, screen->player->pos_y);
	while (x < screen->width)
	{
		ft_init_camera(screen, &cam, x);
		ray_dir = cam.ray_dir;
		map = cam.map;
		delta_dist = cam.delta_dist;
		dist = cam.dist;
		step = cam.step;
//		printf("CHECKING FOR HITS\n");
		hit = 0;
		while (hit == 0)
		{
			if (dist.x < dist.y)
			{
				dist.x += delta_dist.x;
				map.x += step.x;
				side_check = 0;
			}
			else
			{
				dist.y += delta_dist.y;
				map.y += step.y;
				side_check = 1;
			}
			//	if (screen.map_data[map.x][map.y] > 0)
			if (map.y > screen->map_height || map.x > screen->map_width || map.x < 0 || map.y < 0)
			{
				ft_printf("About to crash from illegal map index\n");
				printf("mapx,y = (%d, %d)\n", map.x, map.y);
				//		printf("avoiding the crash\n");
				//		break; //bad idea
			}
			if (screen->map[map.y][map.x] == 1)
			{
				hit = 1;
//				printf("GOT A HIT\n");
			}
		}
		if (side_check == 0)
			wall_dist = (map.x - screen->player->pos_x + (1 - step.x) / 2) / ray_dir.x;
		else
			wall_dist = (map.y - screen->player->pos_y + (1 - step.y) / 2) / ray_dir.y;
		double wall_x; //where on wall hit occured
		if (side_check == 0)
			wall_x = screen->player->pos_y + wall_dist * ray_dir.y;
		else
			wall_x = screen->player->pos_x + wall_dist * ray_dir.x;
//		printf("WALL_X = %f  (%f)\n", wall_x, floor(wall_x));
		wall_x -= floor((wall_x));

		int	wall_num = 0;
		if (side_check == 0)
		{
			if (ray_dir.x < 0)
				wall_num = 1; //0 == N, 1 == E, etc
			else
				wall_num = 3;
		}
		else if (ray_dir.y < 0)
			wall_num = 2;
		int texture_width;
		int		texture_height;
		unsigned int color;
		texture_width = screen->walls[wall_num].width;
		texture_height = screen->walls[wall_num].height;

		int texture_x;
		texture_x = (int)(wall_x * (double)texture_width);
//		ft_printf("texture_x = %d\n", texture_x);
		if (side_check == 0 && ray_dir.x > 0)
			texture_x = texture_width - texture_x - 1;
		else if (side_check == 1 && ray_dir.y < 0)
			texture_x = texture_width - texture_x - 1;
//		ft_printf("iafter texture_x = %d\n", texture_x);

		double	step;
		int		line_height;
		int		draw_start;
		int		draw_end;
		double	texture_pos;
		line_height = (int)(screen->height / wall_dist);
		step = 1.0 * texture_height / line_height;
		draw_start = (screen->height - line_height) / 2;
		draw_end = (screen->height + line_height) / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= screen->height)
			draw_end = screen->height - 1;
		texture_pos = (draw_start - (screen->height - line_height) / 2) * step;
		int y;
		int texture_y;
		y = draw_start;
		while (y < draw_end)
		{
			texture_y = (int)texture_pos & (texture_height - 1); //what's this masking doing?
			texture_pos += step;
			color = ft_get_pixel_from_image(&(screen->walls[wall_num]), texture_x, texture_y);
			ft_pixel_put(&(screen->buf), x, y, color);
			++y;
		}
		sprite_buffer[x] = wall_dist;
		++x;
	}
	printf("Leaving raycast wall loop\n");
	printf("Trying to draw sprites\n");
	if (ft_cast_sprites(screen, cam.plane, sprite_buffer) == -1)
		return (-1); //THIS IS AN ERROR, probably malloc fail make sure when draw() calls raycast it checks for this problm
	return (0);
}
