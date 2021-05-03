/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_to_screen.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 20:31:00 by tmorris          ###   ########.fr       */
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

void			ft_sprite_list_to_array(t_screen *screen, t_map_data *data)
{
	int			i;
	t_list		*index;
	t_sprite	*node;

	i = 0;
	index = data->sprite_list;
	while (index && i < screen->sprite_count)
	{
		node = (t_sprite*)index->content;
		screen->sprites[i].x = node->x;
		screen->sprites[i].y = node->y;
		index = index->next;
		i++;
	}
}

void			ft_parse_to_screen(t_screen *screen, t_map_data *data)
{
	screen->player->pos_x = data->player_x + 0.01;
	screen->player->pos_y = data->player_y + 0.01;
	screen->player->rot_x = data->player_facing_x;
	screen->player->rot_y = data->player_facing_y;
	screen->player->hp = 20;
	screen->colors.ceiling = data->ceil;
	screen->colors.floor = data->floor;
	screen->map_height = data->map_height;
	screen->map_width = data->map_width;
	screen->map = data->map_grid;
	data->map_grid = NULL;
	screen->sprite_count = data->sprite_count;
	screen->sprites = malloc(sizeof(t_sprite) * data->sprite_count);
	if (!screen->sprites)
		ft_free_map_exit(screen, data, "Couldn't allocate sprite array");
	ft_sprite_list_to_array(screen, data);
	screen->width = data->res_width;
	screen->height = data->res_height;
	screen->refresh = 1;
	screen->mlx = mlx_init();
	if (!screen->mlx)
		ft_free_map_exit(screen, data, "Couldn't connect to X server");
}

static void		ft_load_textures(t_map_data *data, t_screen *screen)
{
	int		i;

	i = -1;
	while (++i < 4)
	{
		if (ft_load_image(data->textures[i], screen, &(screen->walls[i])) == -1)
			ft_free_map_exit(screen, data, "Wall texture not loadable as XPM");
	}
	if (ft_load_image(data->sprite, screen, &(screen->sprite)) == -1)
		ft_free_map_exit(screen, data, "Sprite texture not loadable as XPM");
}

t_screen		*ft_load_screen(t_player *player, t_map_data *data, int save)
{
	t_screen	*screen;

	if (!(screen = ft_new_screen(player)))
		return (NULL);
	ft_parse_to_screen(screen, data);
	if (save == FALSE)
		ft_reset_resolution(screen);
	screen->buf.img = mlx_new_image(screen->mlx, screen->width,
			screen->height);
	if (!screen->buf.img)
		ft_free_map_exit(screen, data, "Couldn't create buffer image");
	screen->buf.addr = mlx_get_data_addr(screen->buf.img, &screen->buf.bpp,
			&screen->buf.len, &screen->buf.endian);
	if (!screen->buf.addr)
		ft_free_map_exit(screen, data, "Couldn't get buffer address");
	screen->buf.width = screen->width;
	screen->buf.height = screen->height;
	ft_load_textures(data, screen);
	return (screen);
}
