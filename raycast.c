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
	double	screen_x;
	double	*buffer;
}				t_sprite_data;

void	ft_sort_sprites(int *order, double *dist, int amount)
{
	int		i;
	int		hold_order;
	double	hold_dist;

	i = 0;
	while (i < amount - 1)
	{
		if (dist[i] < dist[i + 1])
		{
			hold_order = order[i + 1];
			hold_dist = dist[i + 1];
			order[i + 1] = order[i];
			order[i] = hold_order;
			dist[i + 1] = dist[i];
			dist[i] = hold_dist;
		}
		++i;
	}
}

void	ft_draw_sprite(t_screen *screen, t_seg draw, t_sprite_data *sprite_data, double transformY)
{
	int		x; //rename to i?
		x = draw.start.x;
		while (x < draw.end.x)
		{
			int texX = (256 * (x - (-sprite_data->dim.x / 2 + sprite_data->screen_x)) * screen->sprite.width / sprite_data->dim.x) / 256;
			//printf("checking if sprite is onscreen\n");
			if (x > 0 && x < screen->width && transformY > 0 && transformY < sprite_data->buffer[x])
			{
				//printf("Here we go!!! drawing the sprite\n");
				int y = draw.start.y;
				while (y < draw.end.y)
				{
					int d = y * 256 - screen->height * 128 + sprite_data->dim.y * 128;
					int texY = ((d * screen->sprite.height) / sprite_data->dim.y) / 256;

					int color = ft_get_pixel_from_image(&(screen->sprite), texX, texY);
					if ((color & 0x00FFFFFF) != 0)
					{
						ft_pixel_put(&(screen->buf), x, y, color);
					}
					++y;
				}
			}
			++x;
		}
}

void	ft_cast_sprites(t_screen *screen, t_d_pt plane, double *sprite_buffer)
{
	int		x; //could be named something else, like i, to be more clear?
	int		spriteOrder[screen->sprite_count]; //can't use numSprites.. malloc
	double	spriteDistance[screen->sprite_count]; //can't use numSprites.. malloc
	x = 0;
	while (x < screen->sprite_count) 
	{
		spriteOrder[x] = x;
		spriteDistance[x] = ((screen->player->pos_x - screen->sprites[x].x) * (screen->player->pos_x - screen->sprites[x].x) + (screen->player->pos_y - screen->sprites[x].y) * (screen->player->pos_y - screen->sprites[x].y));
		++x;
	}
	ft_sort_sprites(spriteOrder, spriteDistance, screen->sprite_count); //can't use these vars
	int currentSprite = 0;
	while (currentSprite < screen->sprite_count)
	{
		double spriteX = screen->sprites[spriteOrder[currentSprite]].x - screen->player->pos_x;
		double spriteY = screen->sprites[spriteOrder[currentSprite]].y - screen->player->pos_y;

		double invDet = 1.0 / (plane.x * screen->player->rot_y - screen->player->rot_x * plane.y);
		double transformX = invDet * (screen->player->rot_y * spriteX - screen->player->rot_x * spriteY);
		double transformY = invDet * (-plane.y *spriteX + plane.x * spriteY);
		t_sprite_data	sprite_data;
		sprite_data.screen_x = (int)((screen->width / 2) * (1 + transformX / transformY));
		sprite_data.dim.y = (int)(abs(screen->height / transformY));
		sprite_data.buffer = sprite_buffer;
		t_seg	draw;
		draw.start.y = -sprite_data.dim.y / 2 + screen->height / 2;
		if (draw.start.y < 0) draw.start.y = 0;
		draw.end.y = sprite_data.dim.y / 2 + screen->height / 2;
		if (draw.end.y >= screen->height) draw.end.y = screen->height - 1;

		sprite_data.dim.x = (int)(abs(screen->height / transformY));
		draw.start.x = (int)(-sprite_data.dim.x / 2 + sprite_data.screen_x);
		if (draw.start.x < 0) draw.start.x = 0;
		draw.end.x = (int)(sprite_data.dim.x / 2 + sprite_data.screen_x);
		if (draw.end.x >= screen->width) draw.end.x = screen->width - 1;

		printf("Drawing sprite #%d (sprite_data.dim.x=%d, draw_start.x=%d, draw_end.x=%d)\n", currentSprite, sprite_data.dim.x, draw.start.x, draw.end.x);
		printf("sprite x = %f, sprite y = %f\n", screen->sprites[currentSprite].x, screen->sprites[currentSprite].y);
		ft_draw_sprite(screen, draw, &sprite_data, transformY);
		++currentSprite;
	}
	printf("done drawing sprites\n");
}

void	ft_raycast(t_screen *screen)
{
	int		x;
	t_d_pt	plane; //camera plane
	double	camera_x;
	t_d_pt	ray_dir;
	t_d_pt	delta_dist;
	t_pt	map;
	t_d_pt	dist; //side_dist
	t_pt	step;
	double	wall_dist;
	int		hit;
	int		side_check;
	double	sprite_buffer[screen->width]; //must be malloc'd instead
	
	plane.x = -.50 * screen->player->rot_y;
	plane.y = (-.50 *  (-screen->player->rot_x));
	printf("<%f, %f>\n", plane.x, plane.y);
	x = 0;
	printf("START LOOP with player location = < %f, %f >\n", screen->player->pos_x, screen->player->pos_y);
	while (x < screen->width)
	{
		camera_x = 2 * x / (double)screen->width - 1;
		ray_dir.x = screen->player->rot_x + plane.x * camera_x;
		ray_dir.y = screen->player->rot_y + plane.y * camera_x;

		map.x = (int)(screen->player->pos_x);
		map.y = (int)(screen->player->pos_y);
		
		if (ray_dir.y == 0)
			delta_dist.x = 0;
		else if (ray_dir.x == 0)
			delta_dist.x = 1;
		else
			delta_dist.x = fabs(1 / ray_dir.x);
		if (ray_dir.x == 0)
			delta_dist.y = 0;
		else if (ray_dir.y == 0)
			delta_dist.y = 1;
		else
			delta_dist.y = fabs(1 / ray_dir.y);
		
		if (ray_dir.x < 0)
		{
			step.x = -1;
			dist.x = (screen->player->pos_x - map.x) * delta_dist.x;
		}
		else
		{
			step.x = 1;
			dist.x = (map.x + 1.0 - screen->player->pos_x) * delta_dist.x;
		}
		if (ray_dir.y < 0)
		{
			step.y = -1;
			dist.y = (screen->player->pos_y - map.y) * delta_dist.y;
		}
		else
		{
			step.y = 1;
			dist.y = (map.y + 1.0 - screen->player->pos_y) * delta_dist.y;
		}
//		printf("CHECVING FOR HITS\n");
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
			else if (screen->map[map.y][map.x] == 2)
			{
				//printf("GOT A SPRITE HIT\n");
				//tempSpriteHit = 2;
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
	ft_cast_sprites(screen, plane, sprite_buffer);
}
