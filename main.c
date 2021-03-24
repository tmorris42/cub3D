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

#include "cub3d.h"

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

void			ft_parse_to_screen(t_screen *screen, t_map_data *data)
{
	t_player	*player;
	int			i;
	t_list		*index;
	t_sprite	*node;

	player = screen->player;
	player->pos_x = data->player_x + 0.01;
	player->pos_y = data->player_y + 0.01;
	player->rot_x = data->player_facing_x;
	player->rot_y = data->player_facing_y;
	screen->colors.ceiling = data->ceil;
	screen->colors.floor = data->floor;
	screen->map_height = data->map_height;
	screen->map_width = data->map_width;
	screen->map = data->map_grid;
	data->map_grid = NULL;
	screen->sprite_count = data->sprite_count;
	screen->sprites = malloc(sizeof(t_sprite) * data->sprite_count);
	if (!screen->sprites)
		ft_free_map_exit(screen, data, "Could not allocate sprite array");
	i = 0;
	index = data->sprite_list;
	while (i < screen->sprite_count)
	{
		node = (t_sprite*)index->content;
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
		ft_free_map_exit(screen, data, "Could not connect to X server");
}

t_screen		*ft_load_screen(t_player *player, t_map_data *data, int save)
{
	int			i;
	t_screen	*screen;

	screen = ft_new_screen(player);
	if (!screen)
		return (NULL);
	ft_parse_to_screen(screen, data);
	// is the below screen->mlx check redundant?
	if (!screen->mlx)
		ft_free_map_exit(screen, data, "Could not connect to X Server");
	if (save == FALSE)
		ft_reset_resolution(screen);
	screen->buf.img = mlx_new_image(screen->mlx, screen->width,
			screen->height);
	if (!screen->buf.img)
		ft_free_map_exit(screen, data, "Could not create buffer image");
	screen->buf.addr = mlx_get_data_addr(screen->buf.img, &screen->buf.bpp,
			&screen->buf.len, &screen->buf.endian);
	screen->buf.width = screen->width;
	screen->buf.height = screen->height;
	i = -1;
	while (++i < 4)
	{
		if (ft_load_image(data->textures[i], screen, &(screen->walls[i])) == -1)
			ft_free_map_exit(screen, data, "Could not load wall texture");
	}
	if (ft_load_image(data->sprite, screen, &(screen->sprite)) == -1)
		ft_free_map_exit(screen, data, "Could not load sprite texture");
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

void			ft_parse_options(int argc, char **argv, t_screen *screen)
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
	if (argc == 2)
		screen = ft_load_screen(&player, map_parse, FALSE);
	else
		screen = ft_load_screen(&player, map_parse, TRUE);
	map_parse = ft_free_map_data(map_parse);
	if (!screen)
		ft_close_on_error(screen, "Error\nCould not initialize screen");
	ft_parse_options(argc, argv, screen);
	return (ft_run(screen));
}
