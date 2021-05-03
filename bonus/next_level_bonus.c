/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_level_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 11:38:43 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/04 00:17:49 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			cleanse_screen(t_screen **screen_addr)
{
	t_screen	*screen;
	int			i;

	if (!screen_addr || (!(*screen_addr)))
		exit(EXIT_SUCCESS);
	screen = *screen_addr;
	if (screen->map)
		ft_free_int_array(screen->map, screen->map_height);
	i = 0;
	free(screen->next_level);
	while (screen->mlx && screen->walls && i < 4 && screen->walls[i].img)
		mlx_destroy_image(screen->mlx, screen->walls[i++].img);
	if ((screen->mlx) && (screen->sprite.img))
		mlx_destroy_image(screen->mlx, screen->sprite.img);
	free(screen->sprites);
	screen->sprites = NULL;
	return (1);
}

void		ft_reinit_screen(t_screen *screen)
{
	int		i;

	screen->colors.ceiling = 0;
	screen->colors.floor = 0;
	screen->map_height = 0;
	screen->map_width = 0;
	screen->map = NULL;
	ft_init_keys(&screen->keys);
	screen->refresh = 1;
	screen->sprite_count = 0;
	screen->sprites = NULL;
	ft_init_image_data(&screen->sprite);
	i = 0;
	while (i < 4)
	{
		ft_init_image_data(&screen->walls[i]);
		++i;
	}
}

void		reparse_to_screen(t_screen *screen, t_map_data *data)
{
	screen->player->pos_x = data->player_x + 0.01;
	screen->player->pos_y = data->player_y + 0.01;
	screen->player->rot_x = data->player_facing_x;
	screen->player->rot_y = data->player_facing_y;
	screen->colors.ceiling = data->ceil;
	screen->colors.floor = data->floor;
	screen->map_height = data->map_height;
	screen->map_width = data->map_width;
	screen->map = data->map_grid;
	screen->next_level = data->next_level;
	data->next_level = NULL;
	data->map_grid = NULL;
	screen->sprite_count = data->sprite_count;
	screen->sprites = malloc(sizeof(t_sprite) * data->sprite_count);
	if (!screen->sprites)
		ft_free_map_exit(screen, data, "Couldn't allocate sprite array");
	ft_sprite_list_to_array(screen, data);
	screen->refresh = 1;
}

t_screen	*reload_screen(t_screen *screen, t_map_data *data)
{
	reparse_to_screen(screen, data);
	if (!screen->buf.addr)
		ft_free_map_exit(screen, data, "Couldn't get buffer address");
	ft_load_textures(data, screen);
	return (screen);
}

t_screen	*next_level(t_screen *screen)
{
	t_map_data	*map_parse;

	if (!screen)
		return (NULL);
	map_parse = ft_parse_file(screen->next_level);
	if (!map_parse)
		ft_close_on_error(screen, "Failed to load next map");
	cleanse_screen(&screen);
	ft_reinit_screen(screen);
	screen = reload_screen(screen, map_parse);
	map_parse = ft_free_map_data(map_parse);
	return (screen);
}
