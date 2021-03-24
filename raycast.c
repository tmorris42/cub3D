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

typedef struct	s_wall_data
{
	int		num;
	double	x;
	double	dist;
	int		side_check;
}				t_wall_data;

typedef struct	s_texture
{
	int		x;
	int		y;
	double	pos;
}				t_texture;

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

void	ft_init_camera(t_screen *scr, t_camera *cam, int x)
{
	double	cam_x;

	cam_x = 2 * x / (double)scr->width - 1;
	cam->ray_dir.x = scr->player->rot_x + cam->plane.x * cam_x;
	cam->ray_dir.y = scr->player->rot_y + cam->plane.y * cam_x;
	cam->map.x = (int)(scr->player->pos_x);
	cam->map.y = (int)(scr->player->pos_y);
	cam->delta_dist.x = fabs(1 / cam->ray_dir.x);
	if (cam->ray_dir.y == 0)
		cam->delta_dist.x = 0;
	else if (cam->ray_dir.x == 0)
		cam->delta_dist.x = 1;
	cam->delta_dist.y = fabs(1 / cam->ray_dir.y);
	if (cam->ray_dir.x == 0)
		cam->delta_dist.y = 0;
	else if (cam->ray_dir.y == 0)
		cam->delta_dist.y = 1;
	cam->step.x = (-2 * (cam->ray_dir.x < 0)) + 1;
	cam->dist.x = (cam->map.x + 1.0 - scr->player->pos_x) * cam->delta_dist.x;
	if (cam->ray_dir.x < 0)
		cam->dist.x = (scr->player->pos_x - cam->map.x) * cam->delta_dist.x;
	cam->step.y = (-2 * (cam->ray_dir.y < 0)) + 1;
	cam->dist.y = (cam->map.y + 1.0 - scr->player->pos_y) * cam->delta_dist.y;
	if (cam->ray_dir.y < 0)
		cam->dist.y = (scr->player->pos_y - cam->map.y) * cam->delta_dist.y;
}

void	ft_find_next_wall(t_screen *screen, t_camera *cam, t_wall_data *wall)
{
	int		hit;

	hit = 0;
	while (hit == 0)
	{
		if (cam->dist.x < cam->dist.y)
		{
			cam->dist.x += cam->delta_dist.x;
			cam->map.x += cam->step.x;
			wall->side_check = 0;
		}
		else
		{
			cam->dist.y += cam->delta_dist.y;
			cam->map.y += cam->step.y;
			wall->side_check = 1;
		}
		if (cam->map.y > screen->map_height || cam->map.x > screen->map_width
				|| cam->map.x < 0 || cam->map.y < 0)
			return ; //Error, no wall found //this will crash;
		if (screen->map[cam->map.y][cam->map.x] == 1)
			hit = 1;
	}
}

void	ft_get_wall(t_screen *screen, t_camera *cam, t_wall_data *wall)
{
	if (wall->side_check == 0)
		wall->dist = (cam->map.x - screen->player->pos_x +
				(1 - cam->step.x) / 2) / cam->ray_dir.x;
	else
		wall->dist = (cam->map.y - screen->player->pos_y +
				(1 - cam->step.y) / 2) / cam->ray_dir.y;
	if (wall->side_check == 0)
		wall->x = screen->player->pos_y + wall->dist * cam->ray_dir.y;
	else
		wall->x = screen->player->pos_x + wall->dist * cam->ray_dir.x;
	wall->x -= floor((wall->x));
	wall->num = 0;
	if (wall->side_check == 0)
	{
		if (cam->ray_dir.x < 0)
			wall->num = 1;
		else
			wall->num = 3;
	}
	else if (cam->ray_dir.y < 0)
		wall->num = 2;
}

void	ft_draw_wall(t_screen *screen, t_camera *cam, t_wall_data *wall, int x)
{
	t_texture		text;
	unsigned int	color;
	t_pt			draw;
	double			step;
	int				line_height;

	text.x = (int)(wall->x * (double)screen->walls[wall->num].width);
	if (wall->side_check == 0 && cam->ray_dir.x > 0)
		text.x = screen->walls[wall->num].width - text.x - 1;
	else if (wall->side_check == 1 && cam->ray_dir.y < 0)
		text.x = screen->walls[wall->num].width - text.x - 1;
	line_height = (int)(screen->height / wall->dist);
	step = 1.0 * screen->walls[wall->num].height / line_height;
	draw.x = ft_max(((screen->height - line_height) / 2), 0);
	draw.y = ft_min((screen->height + line_height) / 2, screen->height - 1);
	text.pos = (draw.x - (screen->height - line_height) / 2) * step;
	while (draw.x < draw.y)
	{
		text.y = (int)text.pos & (screen->walls[wall->num].height - 1);
		text.pos += step;
		color = ft_get_pixel_from_image(&(screen->walls[wall->num]),
				text.x, text.y);
		ft_pixel_put(&(screen->buf), x, draw.x, color);
		++draw.x;
	}
}

void	ft_free_sprite_data(t_sprite_data *data)
{
	free(data->order);
	free(data->dist);
	free(data->buffer);
	data->order = NULL;
	data->dist = NULL;
	data->buffer = NULL;
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

int		ft_raycast(t_screen *screen)
{
	int				x;
	t_camera		cam;
	t_wall_data		wall;
	t_sprite_data	sprite_data;

	ft_init_sprite_data(&sprite_data);
	if (!(sprite_data.buffer = (double*)malloc(sizeof(double) * screen->width)))
		return (-1); //make sure we're checking for raycast failure
	cam.plane.x = -.50 * screen->player->rot_y;
	cam.plane.y = (-.50 * (-screen->player->rot_x));
	x = -1;
	while (++x < screen->width)
	{
		ft_init_camera(screen, &cam, x);
		ft_find_next_wall(screen, &cam, &wall);
		ft_get_wall(screen, &cam, &wall);
		ft_draw_wall(screen, &cam, &wall, x);
		sprite_data.buffer[x] = wall.dist;
	}
	x = 0;
	sprite_data.plane = cam.plane;
	if (ft_cast_sprites(screen, &sprite_data) == -1)
		x = -1; //ERROR make sure draw() checks for this on raycast() call
	ft_free_sprite_data(&sprite_data);
	return (x);
}
