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

int		ft_find_next_wall(t_screen *screen, t_camera *cam, t_wall_data *wall)
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
			return (-1);
		if (screen->map[cam->map.y][cam->map.x] == 1)
			hit = 1;
	}
	return (0);
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
	if (wall->side_check == 0 && cam->ray_dir.x < 0)
		text.x = screen->walls[wall->num].width - text.x - 1;
	else if (wall->side_check == 1 && cam->ray_dir.y > 0)
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

int		ft_raycast(t_screen *screen)
{
	int				x;
	t_camera		cam;
	t_wall_data		wall;
	t_sprite_data	sprite_data;

	ft_init_sprite_data(&sprite_data);
	if (!(sprite_data.buffer = (double*)malloc(sizeof(double) * screen->width)))
		return (-1);
	cam.plane.x = -.50 * screen->player->rot_y;
	cam.plane.y = (-.50 * (-screen->player->rot_x));
	x = -1;
	while (++x < screen->width)
	{
		ft_init_camera(screen, &cam, x);
		if ((ft_find_next_wall(screen, &cam, &wall)) == -1)
			return (ft_free_sprite_data(&sprite_data));
		ft_get_wall(screen, &cam, &wall);
		ft_draw_wall(screen, &cam, &wall, x);
		sprite_data.buffer[x] = wall.dist;
	}
	sprite_data.plane = cam.plane;
	if (ft_cast_sprites(screen, &sprite_data) == -1)
		return (ft_free_sprite_data(&sprite_data));
	ft_free_sprite_data(&sprite_data);
	return (0);
}
