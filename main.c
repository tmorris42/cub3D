/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 18:03:31 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

#define K_ESCAPE	65307
#define K_LEFT		65361
#define K_UP		65362
#define K_RIGHT		65363
#define K_DOWN		65364
#define K_W			119
#define K_A			97
#define K_S			115
#define K_D			100

#define RED			0xff0000
#define GREEN		0x00ff00
#define BLUE		0x0000ff
#define DARKGRAY	1315860
#define LIGHTGRAY	13158600

#define TRUE		1
#define FALSE		0

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

int				ft_parse_keys(int key, t_screen *screen)
{
	t_player	*p;

	p = screen->player;
	if (key == K_ESCAPE)
		ft_close_screen(&screen);
	else if (key == K_UP || key == K_W)
		ft_move_relative(p->rot_x, p->rot_y, screen);
	else if (key == K_DOWN || key == K_S)
		ft_move_relative(-p->rot_x, -p->rot_y, screen);
	else if (key == K_A)
		ft_move_relative(p->rot_y, -p->rot_x, screen);
	else if (key == K_D)
		ft_move_relative(-p->rot_y, p->rot_x, screen);
	else if (key == K_RIGHT)
		ft_rotate(M_PI / 6.0, screen);
	else if (key == K_LEFT)
		ft_rotate(-M_PI / 6.0, screen);
	else
		printf(" : %d\n", key);  //for debugging only, REMOVE THIS
	screen->refresh = 1;
	return (1);
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
	screen->sprites[0] = data->sprites[0]; // should be a dynamic malloc thing when finished. just for testing.
	screen->player = player;
	screen->width = data->res_width;
	screen->height = data->res_height;
	screen->refresh = 1;
	screen->mlx = mlx_init();
}

t_screen		*ft_load_screen(t_player *player, t_map_data *data, int save)
{
	t_screen	*screen;

	screen = ft_new_screen(player);
	if (!screen)
		return (NULL);
	ft_parse_to_screen(screen, data);
	if (!screen->mlx)
		ft_close_screen(&screen);
	if (save == FALSE)
		ft_reset_resolution(screen);
	screen->buf.img = mlx_new_image(screen->mlx, screen->width,
			screen->height);
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

int				ft_run(t_screen *screen)
{
	screen->win = mlx_new_window(screen->mlx, screen->width,
			screen->height, "cub3D");
	if (!screen->win)
		return (ft_close_screen(&screen));
	mlx_expose_hook(screen->win, &ft_redraw, screen);
	mlx_hook(screen->win, 2, 1L << 0, &ft_parse_keys, screen);
	mlx_hook(screen->win, 33, 0L << 0, &ft_close_screen, &screen);
	mlx_loop_hook(screen->mlx, &ft_draw, screen);
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
	if (!screen)
		ft_close_on_error(screen, "Error\nCould not initialize screen");
	printf("Made it out of load_screen");
	map_parse = ft_free_map_data(map_parse);
	ft_parse_options(argc, argv, screen);
	return (ft_run(screen));
}
