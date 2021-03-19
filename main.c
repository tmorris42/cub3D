/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 23:07:50 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

int				ft_get_color(int r, int g, int b, int alpha)
{
	return ((alpha << 24) + (r << 16) + (g << 8) + (b));
}

void			ft_pixel_put(t_img_data *img, int x, int y, int color)
{
	char			*img_addr;
	int				offset;
	unsigned int	*pixel;

	offset = (y * img->len) + (x * img->bpp / 8);
	img_addr = img->addr + offset;
	pixel = (unsigned int*)img_addr;
	*pixel = color;
}

unsigned int	ft_get_pixel_from_image(t_img_data *img, int x, int y)
{
	char			*img_addr;
	int				offset;
	unsigned int	*color;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
	offset = (y * img->len) + (x * img->bpp / 8);
	img_addr = img->addr + offset;
	color = (unsigned int*)img_addr;
	return (*color);
}

void			ft_rotate(double rad, t_screen *screen)
{
	double		old_rx;
	t_player	*p;

	p = screen->player;
	old_rx = p->rot_x;
	p->rot_x = (cos(rad) * old_rx) - (p->rot_y * sin(rad));
	p->rot_y = (sin(rad) * old_rx) + (cos(rad) * p->rot_y);
}

void			ft_move_relative(double x, double y, t_screen *screen)
{
	t_player	*p;

	p = screen->player;
	if (screen->map_data[(int)(p->pos_y)][(int)(p->pos_x + x)] == 0)
		p->pos_x += x;
	if (screen->map_data[(int)(p->pos_y + y)][(int)p->pos_x] == 0)
		p->pos_y += y;
}

int				ft_parse_keys(int key, t_screen *screen, int pressed)
{
	t_player	*p;

	p = screen->player;
	if (key == K_ESCAPE)
		ft_close_screen(&screen);
	else if (key == K_UP || key == K_W)
	{
			screen->player_move += 8 * pressed;
	}
	else if (key == K_A)
	{
			screen->player_move += 4 * pressed;
	}
	else if (key == K_DOWN || key == K_S)
	{
			screen->player_move += 2 * pressed;
	}
	else if (key == K_D)
	{
			screen->player_move += 1 * pressed;
	}
	else if (key == K_RIGHT)
	{
			screen->player_move += 32 * pressed;
	}
	else if (key == K_LEFT)
	{
			screen->player_move += 16 * pressed;
	}
	else
		printf(" : %d\n", key);  //for debugging only, REMOVE THIS
	screen->refresh = 1;
	return (1);
}

int				ft_press_keys(int key, t_screen *screen)
{
	ft_parse_keys(key, screen, 1);
}

int				ft_lift_keys(int key, t_screen *screen)
{
	ft_parse_keys(key, screen, -1);
}

void			ft_reset_resolution(t_screen *screen)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	mlx_get_screen_size(screen->mlx, &x, &y);
	screen->width = ft_min(screen->width, x);
	screen->height = ft_min(screen->height, y);
}

int				ft_load_image(char *filename, t_screen *screen, t_img_data *image)
{
	image->img = mlx_xpm_file_to_image(screen->mlx, filename, &(image->width),
			&(image->height));
	if (image->img)
	{
		image->addr = mlx_get_data_addr(image->img, &image->bpp, &image->len,
				&image->endian);
		if (!(image->addr))
		{
			mlx_destroy_image(screen->mlx, image->img);
			image->img = NULL;
			return (-1);
		}
	}
	else
		return (-1);
	return (1);
}

void			ft_parse_to_screen(t_screen *screen, t_map_data *data)
{
	t_player	*player;

	player = screen->player;
	player->pos_x = data->player_x + 0.01;
	player->pos_y = data->player_y + 0.01;
	player->rot_x = data->player_facing_x;
	player->rot_y = data->player_facing_y;
	screen->colors.ceiling = data->ceil;
	screen->colors.floor = data->floor;
	screen->map_height = data->map_height;
	screen->map_width = data->map_width;
	screen->map_data = data->map_grid;
	data->map_grid = NULL;
	screen->sprite_num = data->sprite_num;
	screen->sprites = malloc(sizeof(t_sprite) * data->sprite_num);
	if (!screen->sprites)
	{
		ft_free_map_data(data);
		ft_close_screen(&screen); //ERROR HERE
	}
	int	i = 0;
	t_list	*index = data->sprite_list;
	while (i < screen->sprite_num)
	{
		t_sprite *node = (t_sprite*)index->content;
		screen->sprites[i].x = node->x;
		screen->sprites[i].y = node->y;
		index = index->next;
		i++;
	}
	screen->player = player;
	screen->width = data->res_width;
	screen->height = data->res_height;
	screen->refresh = 1;
	screen->mlx = mlx_init();
	if (!screen->mlx)
	{
		ft_free_map_data(data);
		ft_close_screen(&screen); //error creating connection of X server
	}
}

t_screen		*ft_load_screen(t_player *player, t_map_data *data, int save)
{
	t_screen	*screen;

	screen = ft_new_screen(player);
	if (!screen)
		return (NULL);
	ft_parse_to_screen(screen, data);
	if (!screen->mlx) //redundant check?
		ft_close_screen(&screen);
	if (save == FALSE)
		ft_reset_resolution(screen);
	screen->buf.img = mlx_new_image(screen->mlx, screen->width,
			screen->height);
	if (!screen->buf.img)
		return (NULL);
	screen->buf.addr = mlx_get_data_addr(screen->buf.img, &screen->buf.bpp,
			&screen->buf.len, &screen->buf.endian);
	screen->buf.width = screen->width;
	screen->buf.height = screen->height;
	ft_load_image(data->textures[0], screen, &(screen->walls[0]));
	ft_load_image(data->textures[1], screen, &(screen->walls[1]));
	ft_load_image(data->textures[2], screen, &(screen->walls[2]));
	ft_load_image(data->textures[3], screen, &(screen->walls[3]));
	ft_load_image(data->sprite, screen, &(screen->sprite));
	// check that image loaded and is valid
	return (screen);
}

int				ft_update(t_screen *screen)
{
	t_player *p;
	p = screen->player;
	if (screen->player_move == 0)
		return (ft_draw(screen));
	if (screen->player_move & 8)
		ft_move_relative(p->rot_x, p->rot_y, screen);
	if (screen->player_move & 4)
		ft_move_relative(p->rot_y, -p->rot_x, screen);
	if (screen->player_move & 2)
		ft_move_relative(-p->rot_x, -p->rot_y, screen);
	if (screen->player_move & 1)
		ft_move_relative(-p->rot_y, p->rot_x, screen);
	if (screen->player_move & 32)
		ft_rotate(M_PI / 48.0, screen);
	if (screen->player_move & 16)
		ft_rotate(-M_PI / 48.0, screen);
	screen->refresh = 1;	
	return (ft_draw(screen));
}

int				ft_run(t_screen *screen)
{
	screen->win = mlx_new_window(screen->mlx, screen->width,
			screen->height, "cub3D");
	if (!screen->win)
		return (ft_close_screen(&screen));
	mlx_expose_hook(screen->win, &ft_redraw, screen);
	mlx_hook(screen->win, 2, 1L << 0, &ft_press_keys, screen);
	mlx_hook(screen->win, 3, 1L << 1, &ft_lift_keys, screen);
	mlx_hook(screen->win, 33, 0L << 0, &ft_close_screen, &screen);
	mlx_loop_hook(screen->mlx, &ft_update, screen);
	mlx_loop(screen->mlx);
	return (ft_close_screen(&screen));
}

int				ft_parse_options(int argc, char **argv, t_screen *screen)
{
	if (argc > 2)
	{
		if (argc == 3 && !ft_strncmp(argv[2], "--save", 7))
		{
			ft_redraw(screen);
			if (ft_save(screen, "save.bmp") == -1)
				perror("Error\nCould not save screenshot");
			ft_close_screen(&screen);
		}
		else
			ft_close_on_error(screen, "Error\nInvalid Options");
	}
}

void			ft_close_on_error(t_screen *screen, char *msg)
{
	perror(msg);
	ft_close_screen(&screen);
}

int				main(int argc, char **argv)
{
	t_screen	*screen;
	t_player	player;
	t_map_data	*map_parse;

	screen = NULL;
	if (argc > 1)
		map_parse = ft_parse_file(argv[1]);
	else
		ft_close_on_error(screen, "Error\nUsage: ./cub3D <mapname.cub>");
	if (!map_parse)
		ft_close_on_error(screen, "Error\nCould not parse map");
//	ft_print_map_data(map_parse);
	printf("Made it out of second print map data\n");
	if (argc == 2)
		screen = ft_load_screen(&player, map_parse, FALSE);
	else
		screen = ft_load_screen(&player, map_parse, TRUE);
	map_parse = ft_free_map_data(map_parse);
	if (!screen)
		ft_close_on_error(screen, "Error\nCould not initialize screen");
	printf("Made it out of load_screen");
	ft_parse_options(argc, argv, screen);
	return (ft_run(screen));
}
