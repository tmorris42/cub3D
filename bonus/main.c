/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 20:10:39 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int				ft_update(t_screen *screen)
{
	t_player *p;

	p = screen->player;
	if (screen->keys.up)
		ft_move_relative(p->rot_x, p->rot_y, screen);
	if (screen->keys.left)
		ft_move_relative(p->rot_y, -p->rot_x, screen);
	if (screen->keys.down)
		ft_move_relative(-p->rot_x, -p->rot_y, screen);
	if (screen->keys.right)
		ft_move_relative(-p->rot_y, p->rot_x, screen);
	if (screen->keys.turn_right)
		ft_rotate(M_PI / 48.0, screen);
	if (screen->keys.turn_left)
		ft_rotate(-M_PI / 48.0, screen);
	screen->refresh = 1;
	if (ft_draw(screen) == -1)
		ft_close_on_error(screen, "Couldn't allocate memory");
	return (0);
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
				ft_error("Could not save screenshot");
			ft_close_screen(&screen);
		}
		else
			ft_close_on_error(screen, "Invalid Options");
	}
}

int				main(int argc, char **argv)
{
	t_screen	*screen;
	t_player	player;
	t_map_data	*map_parse;

	screen = NULL;
	if (argc == 2 || (argc == 3 && !ft_strncmp(argv[2], "--save", 7)))
		map_parse = ft_parse_file(argv[1]);
	else
		ft_close_on_error(screen, "Usage: ./bonus3D <mapname.cub> [--save]");
	if (!map_parse)
		ft_close_screen(&screen);
	if (argc == 2)
		screen = ft_load_screen(&player, map_parse, FALSE);
	else
		screen = ft_load_screen(&player, map_parse, TRUE);
	map_parse = ft_free_map_data(map_parse);
	if (!screen)
		ft_close_on_error(screen, "Could not initialize screen");
	ft_parse_options(argc, argv, screen);
	return (ft_run(screen));
}
