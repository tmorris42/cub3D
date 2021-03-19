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

void	ft_sort_sprites(int *order, double *dist, int amount)
{
	int	i;
	int hold_order;
	double hold_dist;

	i = 0;
	while (i < amount - 1)
	{
		printf("i=%d, order=%d, dist=%f\n", i, order[i], dist[i]);
		printf("i=%d, order=%d, dist=%f\n", i+1, order[i+1], dist[i+1]);
		if (dist[i] < dist[i+1])
		{
			printf("swapping\n");
			hold_order = order[i+1];
			hold_dist = dist[i+1];
			order[i+1] = order[i];
			order[i] = hold_order;
			dist[i+1] = dist[i];
			dist[i] = hold_dist;
		}
		++i;
	}
	return ;
}

void	ft_raycast(t_screen *screen)
{
	int		x;
	double	plane_x; //camera plane
	double	plane_y; //camera plane
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		map_x;
	int		map_y;
	double	dist_x; //side_dist_x
	double	dist_y;
	double	wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side_check;
	

	double	sprite_buffer[screen->width]; //must be malloc'd instead
	int numSprites = screen->sprite_count;
	int	spriteOrder[numSprites]; //can't use numSprites.. malloc
	double spriteDistance[numSprites]; //can't use numSprites.. malloc
	int tempSpriteHit = 0; // just for debugging
	
//	plane_x = 0;
//	plane_y = -0.66;
	plane_x = -.50 * screen->player->rot_y;
	plane_y = (-.50 *  (-screen->player->rot_x));
	printf("<%f, %f>\n", plane_x, plane_y);

	x = 0;
	printf("START LOOP with player location = < %f, %f >\n", screen->player->pos_x, screen->player->pos_y);
	while (x < screen->width)
	{
		camera_x = 2 * x / (double)screen->width - 1;
		ray_dir_x = screen->player->rot_x + plane_x * camera_x;
		ray_dir_y = screen->player->rot_y + plane_y * camera_x;

		map_x = (int)(screen->player->pos_x);
		map_y = (int)(screen->player->pos_y);
		
		if (ray_dir_y == 0)
			delta_dist_x = 0;
		else if (ray_dir_x == 0)
			delta_dist_x = 1;
		else
			delta_dist_x = fabs(1 / ray_dir_x);
		if (ray_dir_x == 0)
			delta_dist_y = 0;
		else if (ray_dir_y == 0)
			delta_dist_y = 1;
		else
			delta_dist_y = fabs(1 / ray_dir_y);
		
		if (ray_dir_x < 0)
		{
			step_x = -1;
			dist_x = (screen->player->pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			dist_x = (map_x + 1.0 - screen->player->pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			dist_y = (screen->player->pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			dist_y = (map_y + 1.0 - screen->player->pos_y) * delta_dist_y;
		}
//		printf("CHECKING FOR HITS\n");
		hit = 0;
		while (hit == 0)
		{
			if (dist_x < dist_y)
			{
				dist_x += delta_dist_x;
				map_x += step_x;
				side_check = 0;
			}
			else
			{
				dist_y += delta_dist_y;
				map_y += step_y;
				side_check = 1;
			}
		//	if (screen.map_data[map_x][map_y] > 0)
			if (map_y > screen->map_height || map_x > screen->map_width || map_x < 0 || map_y < 0)
			{
				ft_printf("About to crash from illegal map index\n");
				printf("mapx,y = (%d, %d)\n", map_x, map_y);
		//		printf("avoiding the crash\n");
		//		break; //bad idea
			}
			if (screen->map[map_y][map_x] == 1)
			{
				hit = 1;
//				printf("GOT A HIT\n");
			}
			else if (screen->map[map_y][map_x] == 2)
			{
				//printf("GOT A SPRITE HIT\n");
				tempSpriteHit = 2;
			}
		}

		if (side_check == 0)
			wall_dist = (map_x - screen->player->pos_x + (1 - step_x) / 2) / ray_dir_x;
		else
			wall_dist = (map_y - screen->player->pos_y + (1 - step_y) / 2) / ray_dir_y;
		
		double wall_x; //where on wall hit occured
		if (side_check == 0)
			wall_x = screen->player->pos_y + wall_dist * ray_dir_y;
		else
			wall_x = screen->player->pos_x + wall_dist * ray_dir_x;
//		printf("WALL_X = %f  (%f)\n", wall_x, floor(wall_x));
		wall_x -= floor((wall_x));

		int	wall_num = 0;
		if (side_check == 0)
		{
			if (ray_dir_x < 0)
				wall_num = 1; //0 == N, 1 == E, etc
			else
				wall_num = 3;
		}
		else if (ray_dir_y < 0)
			wall_num = 2;
		int texture_width;
		int		texture_height;
		unsigned int color;
		texture_width = screen->walls[wall_num].width;
		texture_height = screen->walls[wall_num].height;

		int texture_x;
		texture_x = (int)(wall_x * (double)texture_width);
//		ft_printf("texture_x = %d\n", texture_x);
		if (side_check == 0 && ray_dir_x > 0)
			texture_x = texture_width - texture_x - 1;
		else if (side_check == 1 && ray_dir_y < 0)
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
		//sort the sprites into distance order here, skipping for now

		++x;
	}
	printf("Leaving raycast wall loop\n");
//	if (tempSpriteHit != 2)
//		return ;
	printf("Trying to draw sprites\n");

	x = 0;
	while (x < numSprites) //can;t use numSprites
	{
		spriteOrder[x] = x;
		spriteDistance[x] = ((screen->player->pos_x - screen->sprites[x].x) * (screen->player->pos_x - screen->sprites[x].x) + (screen->player->pos_y - screen->sprites[x].y) * (screen->player->pos_y - screen->sprites[x].y));
		++x;
	}
	ft_sort_sprites(spriteOrder, spriteDistance, numSprites); //can't use these vars
	int currentSprite = 0;
	while (currentSprite < numSprites) //numSPrites can't use
	{
		double spriteX = screen->sprites[spriteOrder[currentSprite]].x - screen->player->pos_x;
		double spriteY = screen->sprites[spriteOrder[currentSprite]].y - screen->player->pos_y;

		double invDet = 1.0 / (plane_x * screen->player->rot_y - screen->player->rot_x * plane_y);
		double transformX = invDet * (screen->player->rot_y * spriteX - screen->player->rot_x * spriteY);
		double transformY = invDet * (-plane_y *spriteX + plane_x * spriteY);
		int	spriteScreenX = (int)((screen->width / 2) * (1 + transformX / transformY));
		int spriteHeight = (int)(abs(screen->height / transformY));
		int drawStartY = -spriteHeight / 2 + screen->height / 2;
		if (drawStartY < 0) drawStartY = 0;
		int drawEndY = spriteHeight / 2 + screen->height / 2;
		if (drawEndY >= screen->height) drawEndY = screen->height - 1;

		int spriteWidth = (int)(abs(screen->height / transformY));
		int drawStartX = (int)(-spriteWidth / 2 + spriteScreenX);
		if (drawStartX < 0) drawStartX = 0;
		int drawEndX = (int)(spriteWidth / 2 + spriteScreenX);
		if (drawEndX >= screen->width) drawEndX = screen->width - 1;

		printf("Drawing sprite #%d (spriteWidth=%d, drawStartX=%d, drawEndX=%d)\n", currentSprite, spriteWidth, drawStartX, drawEndX);
		printf("sprite x = %f, sprite y = %f\n", screen->sprites[currentSprite].x, screen->sprites[currentSprite].y);
		x = drawStartX;
		while (x < drawEndX)
		{
			int texX = (256 * (x - (-spriteWidth / 2 + spriteScreenX)) * screen->sprite.width / spriteWidth) / 256;
			//printf("checking if sprite is onscreen\n");
			if (transformY > 0 && x > 0 && x < screen->width && transformY < sprite_buffer[x])
			{
				//printf("Here we go!!! drawing the sprite\n");
				int y = drawStartY;
				while (y < drawEndY)
				{
					int d = y * 256 - screen->height * 128 + spriteHeight * 128;
					int texY = ((d * screen->sprite.height) / spriteHeight) / 256;

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
		++currentSprite;
	}
	printf("done drawing sprites\n");
}
