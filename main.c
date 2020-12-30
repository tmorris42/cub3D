#include <stdio.h> //remove this
#include <stdlib.h> //for exit() EXIT_SUCCESS, EXIT_FAILURE
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

int		ft_get_color(int r, int g, int b, int alpha)
{
	return ((alpha << 24) + (r << 16) + (g << 8) + (b));
}

void	ft_pixel_put(t_img_data *img, int x, int y, int color)
{
    char    *img_addr;
	int		offset;
	unsigned int	*pixel;

	offset = (y * img->len) + (x * img->bpp / 8);
	img_addr = img->addr + offset;
	pixel = (unsigned int*)img_addr;
    *pixel = color;
}

unsigned int	ft_get_pixel_from_image(t_img_data *img, int x, int y)
{
    char    *img_addr;
	int		offset;
	unsigned int	*pixel;
	unsigned int	color;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return (0);
//	ft_printf("SIMPLE CHECK SHOWS ILLEGAL\n%d, %d [%d, %d]\n",
//				x, y, img->width, img->height);
	offset = (y * img->len) + (x * img->bpp / 8);
	img_addr = img->addr + offset;
	pixel = (unsigned int*)img_addr;
	color = *pixel;
    return (color);
}

void	ft_draw_vertical_line(t_screen *screen, int x, int y,
								int end, int color)
{
	while (y < end)
	{
		ft_pixel_put(&screen->buf, x, y, color);
		++y;
	}
}

void	ft_draw_rectangle(t_screen *screen, int startx, int starty, int lenx, int leny, int color)
{
	int		i;
	int		j;

	j = starty;
	while (j < starty + leny)
	{
		i = startx;
		while (i < startx + lenx)
		{
			ft_pixel_put(&screen->buf, i, j, color);
			++i;
		}
		++j;
	}
}

void	ft_raycast(t_screen screen)
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
	

//	plane_x = 0;
//	plane_y = -0.66;
	plane_x = -.50 * screen.player->rot_y;
	plane_y = (-.50 *  (-screen.player->rot_x));
	printf("<%f, %f>\n", plane_x, plane_y);

	x = 0;
	printf("START LOOP\n");
	while (x < screen.width)
	{
		camera_x = 2 * x / (double)screen.width - 1;
		ray_dir_x = screen.player->rot_x + plane_x * camera_x;
		ray_dir_y = screen.player->rot_y + plane_y * camera_x;

		map_x = (int)(screen.player->pos_x);
		map_y = (int)(screen.player->pos_y);
		
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
			dist_x = (screen.player->pos_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			dist_x = (map_x + 1.0 - screen.player->pos_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			dist_y = (screen.player->pos_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			dist_y = (map_y + 1.0 - screen.player->pos_y) * delta_dist_y;
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
			if (map_y > screen.map_height || map_x > screen.map_width || map_x < 0 || map_y < 0)
				ft_printf("About to crash from illegal map index");
			if (screen.map_data[map_y][map_x] > 0)
			{
				hit = 1;
//				printf("GOT A HIT\n");
			}
		}

		if (side_check == 0)
			wall_dist = (map_x - screen.player->pos_x + (1 - step_x) / 2) / ray_dir_x;
		else
			wall_dist = (map_y - screen.player->pos_y + (1 - step_y) / 2) / ray_dir_y;
		
		double wall_x; //where on wall hit occured
		if (side_check == 0)
			wall_x = screen.player->pos_y + wall_dist * ray_dir_y;
		else
			wall_x = screen.player->pos_x + wall_dist * ray_dir_x;
//		printf("WALL_X = %f  (%f)\n", wall_x, floor(wall_x));
		wall_x -= floor((wall_x));

		int	wall_num = 0;
		if (side_check == 0)
		{
			if (ray_dir_x < 0)
				wall_num = 3; //0 == N, 1 == E, etc
			else
				wall_num = 1;
		}
		else if (ray_dir_y > 0)
			wall_num = 2;
		int texture_width;
		int		texture_height;
		unsigned int color;
		texture_width = screen.walls[wall_num].width;
		texture_height = screen.walls[wall_num].height;

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
		line_height = (int)(screen.height / wall_dist);
		step = 1.0 * texture_height / line_height;
		
		
		draw_start = (screen.height - line_height) / 2;
		draw_end = (screen.height + line_height) / 2;
		if (draw_start < 0)
			draw_start = 0;
		if (draw_end >= screen.height)
			draw_end = screen.height - 1;
	
		texture_pos = (draw_start - (screen.height - line_height) / 2) * step;
//		texture_pos = 0; //testing this
		int y;
		int texture_y;
		y = draw_start;
		while (y < draw_end)
		{
			texture_y = (int)texture_pos & (texture_height - 1); //what's this masking doing?
			texture_pos += step;
//			ft_printf("texture x,y  %d,%d\n", texture_x, texture_y);
			color = ft_get_pixel_from_image(&(screen.walls[wall_num]), texture_x, texture_y);
			ft_pixel_put(&(screen.buf), x, y, color);
			++y;
		}
		
		++x;
	}

}


int		ft_draw(t_screen *screen)
{
	int		x;
	int		y;
	char	print[6];

	if (screen->refresh == 0)
		return (0);
	screen->refresh = 0;
//	ft_draw_rectangle(screen, 0, 0, screen->width, screen->height, DARKGRAY);
//	ft_draw_rectangle(screen, 0, screen->height / 2, screen->width, screen->height / 2, LIGHTGRAY);
	ft_draw_rectangle(screen, 0, 0, screen->width, screen->height, screen->colors.ceiling);
	ft_draw_rectangle(screen, 0, screen->height / 2, screen->width, screen->height / 2, screen->colors.floor);
//	mlx_clear_window(screen->mlx, screen->win);
	ft_raycast(*screen);
	mlx_put_image_to_window(screen->mlx, screen->win, screen->buf.img, 0, 0);

//	return (1);
	print[5] = '\0';
	y = 0;
	while (y < screen->map_height)
	{
		x = 0;
		while (x < screen->map_width)
		{
//			ft_printf("map_data[%d,%d] = %c\n", x, y, (screen->map_data)[y][x]);
			if ((int)(screen->player->pos_x) == x && (int)(screen->player->pos_y) == y)
				ft_printf("*");
			else
				ft_printf("%c", screen->map_data[y][x] + '0');
			++x;
		}
		ft_printf("\n");
//		mlx_string_put(screen->mlx, screen->win, 10, 10 + 10 * y, 255, print);
		++y;
	}
	printf("Position: %f, %f\nDirection: %f, %f\n",
				screen->player->pos_x, screen->player->pos_y,
				screen->player->rot_x, screen->player->rot_y);
	return (1);
}

int		ft_redraw(t_screen *screen)
{
	screen->refresh = 1;
	return (ft_draw(screen));
}

/*int		ft_close_screen(t_screen *screen)
{
	int		i;

	i = 0;
	while (screen->mlx && screen->walls && i < 4)
		mlx_destroy_image(screen->mlx, screen->walls[i++].img);
	if (screen->mlx && screen->win)
		mlx_destroy_window(screen->mlx, screen->win);
	if (screen->mlx)
		mlx_destroy_display(screen->mlx);
	free(screen->mlx);
	exit(EXIT_SUCCESS);
	
	return (1);
}*/

void	ft_collision_with_wall(t_screen *screen, int x, int y)
{
	int pos_x = (int)(screen->player->pos_x + 0.5);
	int pos_y = (int)(screen->player->pos_y + 0.5);
	if (screen->map_data[pos_x][pos_y] > 0)
	{
		if (x == 1)
			screen->player->pos_x = (double)((int)(screen->player->pos_x) - 0.1);
		else if (x == -1)
		{
			screen->player->pos_x = (double)((int)(screen->player->pos_x + 1) + 0.1);
		}
		if (y == 1)
			screen->player->pos_y = (double)((int)(screen->player->pos_y) - 0.1);
		else if (y == -1)
		{
			screen->player->pos_y = (double)((int)(screen->player->pos_y + 1) + 0.1);
		}
	}
}

int		ft_parse_keys(int	key, t_screen *screen)
{
	double		old_rx;
	t_player	*p;

	p = screen->player;
	if (key == K_ESCAPE)
		ft_close_screen(&screen);
	else if (key == K_UP)
	{
		if (screen->map_data[(int)(p->pos_y)][(int)(p->pos_x + p->rot_x)] == 0)
		{
				screen->player->pos_x += screen->player->rot_x;
		}
		if (screen->map_data[(int)(p->pos_y + p->rot_y)][(int)p->pos_x] == 0)
		{
			screen->player->pos_y += screen->player->rot_y;
		}
	
		/*
		ft_collision_with_wall(screen, -1 + (2 * (screen->player->rot_x > 0)), 0);
		screen->player->pos_y += screen->player->rot_y;
		ft_collision_with_wall(screen, 0, -1 + (2 * (screen->player->rot_y > 0)));
		*/
	//	screen->player->pos_y -= 1;
	}
	else if (key == K_DOWN)
	{
		if (screen->map_data[(int)p->pos_y][(int)(p->pos_x - p->rot_x)] == 0)
			screen->player->pos_x -= screen->player->rot_x;
		if (screen->map_data[(int)(p->pos_y - p->rot_y)][(int)p->pos_x] == 0)
			screen->player->pos_y -= screen->player->rot_y;
		/*
		screen->player->pos_x -= screen->player->rot_x;
		ft_collision_with_wall(screen, -1 + (2 * (screen->player->rot_x < 0)), 0);
		screen->player->pos_y -= screen->player->rot_y;
		ft_collision_with_wall(screen, 0, -1 + (2 * (screen->player->rot_y < 0)));
*/
//		screen->player->pos_y += 1;
	}
	else if (key == K_RIGHT)	//should be turn right
	{
		old_rx = screen->player->rot_x;
		screen->player->rot_x = (0.866 * old_rx) - (screen->player->rot_y / 2);
		screen->player->rot_y = (0.5 * old_rx) + (0.866 * screen->player->rot_y);

//		screen->player->pos_x += 1;
	}
	else if (key == K_LEFT) //should be turn left
	{
		old_rx = screen->player->rot_x;
		screen->player->rot_x = 0.866 * old_rx + (screen->player->rot_y / 2);
		screen->player->rot_y = (-0.5 * old_rx) + (0.866 * screen->player->rot_y);
//		screen->player->pos_x += 1;
	}
	else
		printf(" : %d\n", key);  //for debugging only, REMOVE THIS
	screen->refresh = 1;
	return (1);
}

int		main(int argc, char **argv)
{
	t_screen	*screen;
	t_player	player;
	int			texture_width;
	int			texture_height;
	t_map_data	*map_parse;

	if (argc > 1)
		map_parse = ft_parse_file(argv[1]);
	else
		return (0);

	if (map_parse)
		ft_print_map_data(*map_parse);
	else
		return (-1);
	ft_printf("player x: %d\n", map_parse->player_x);
	screen = ft_new_screen();
	if (!screen)
		perror("Error\nScreen could not be initialized");
	player.pos_x = map_parse->player_x + 0.01;
	player.pos_y = map_parse->player_y + 0.01;
	player.rot_x = map_parse->player_facing_x;
	player.rot_y = map_parse->player_facing_y;
	screen->colors.ceiling = map_parse->ceil;
	screen->colors.floor = map_parse->floor;
	screen->map_height = map_parse->map_height;
	screen->map_width = map_parse->map_width;
	screen->map_data = map_parse->map_grid;
	screen->player = &player;
	screen->width = map_parse->res_width;
	screen->height = map_parse->res_height;
	screen->refresh = 1;
	screen->mlx = mlx_init();
	if (!screen->mlx)
		return (0);
	screen->win = mlx_new_window(screen->mlx, screen->width, screen->height, "cub3D");
	if (!screen->win)
		return (0); // not freeing from mlx_init
	screen->buf.img  = mlx_new_image(screen->mlx, screen->width, screen->height);
	screen->buf.addr = mlx_get_data_addr(screen->buf.img, &screen->buf.bpp, &screen->buf.len, &screen->buf.endian);
	screen->buf.width = screen->width;
	screen->buf.height = screen->height;
	ft_printf("BUFFER SIZE: %d,%d\n", screen->buf.width, screen->buf.height);

	texture_width = 0;
	texture_height = 0;

	screen->walls[0].img = mlx_xpm_file_to_image(screen->mlx, map_parse->textures[0], &(screen->walls[0].width), &(screen->walls[0].height));
	screen->walls[1].img = mlx_xpm_file_to_image(screen->mlx, map_parse->textures[1], &(screen->walls[1].width), &(screen->walls[1].height));
	screen->walls[2].img = mlx_xpm_file_to_image(screen->mlx, map_parse->textures[2], &(screen->walls[2].width), &(screen->walls[2].height));
	screen->walls[3].img = mlx_xpm_file_to_image(screen->mlx, map_parse->textures[3], &(screen->walls[3].width), &(screen->walls[3].height));

	screen->walls[0].addr = mlx_get_data_addr(screen->walls[0].img, &screen->walls[0].bpp, &screen->walls[0].len, &screen->walls[0].endian);
	screen->walls[1].addr = mlx_get_data_addr(screen->walls[1].img, &screen->walls[1].bpp, &screen->walls[1].len, &screen->walls[1].endian);
	screen->walls[2].addr = mlx_get_data_addr(screen->walls[2].img, &screen->walls[2].bpp, &screen->walls[2].len, &screen->walls[2].endian);
	screen->walls[3].addr = mlx_get_data_addr(screen->walls[3].img, &screen->walls[3].bpp, &screen->walls[3].len, &screen->walls[3].endian);
	// check that image loaded and is valid

	if (argc > 2)
	{
		if (argc == 3 && !ft_strncmp(argv[2], "--save", 7))
		{
			ft_redraw(screen);
			ft_save(screen, "save.bmp"); //if returns -1, then error
			ft_close_screen(&screen);
		}
		else
		{
			ft_printf("INVALID OPTION -- usage -- blah blah"); //clear and exit
			ft_close_screen(&screen);
		}
	}

	mlx_expose_hook(screen->win, &ft_redraw, screen);
	mlx_hook(screen->win, 2, 1L<<0, &ft_parse_keys, screen);
	mlx_hook(screen->win, 33, 0L<<0, &ft_close_screen, &screen);
	mlx_loop_hook(screen->mlx, &ft_draw, screen);

	// Wait so user can see window
	mlx_loop(screen->mlx);
	
//	mlx_destroy_window(screen.mlx, screen.win);
	ft_close_screen(&screen);
	exit(EXIT_SUCCESS);
}
