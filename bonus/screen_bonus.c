/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 11:38:43 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 23:12:14 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int			ft_close_screen(t_screen **screen_addr)
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
	if ((screen->mlx) && (screen->buf.img))
		mlx_destroy_image(screen->mlx, screen->buf.img);
	if ((screen->mlx) && (screen->sprite.img))
		mlx_destroy_image(screen->mlx, screen->sprite.img);
	if (screen->mlx && screen->win)
		mlx_destroy_window(screen->mlx, screen->win);
	if (screen->mlx)
		mlx_destroy_display(screen->mlx);
	free(screen->sprites);
	free(screen->mlx);
	free(screen);
	exit(EXIT_SUCCESS);
	return (1);
}

void		ft_init_image_data(t_img_data *img)
{
	img->img = NULL;
	img->width = 0;
	img->height = 0;
	img->addr = NULL;
	img->bpp = 0;
	img->len = 0;
	img->endian = 0;
}

void		ft_init_keys(t_keys *keys)
{
	keys->up = FALSE;
	keys->down = FALSE;
	keys->left = FALSE;
	keys->right = FALSE;
	keys->turn_right = FALSE;
	keys->turn_left = FALSE;
}

void		ft_init_screen(t_screen *screen)
{
	int		i;

	screen->colors.ceiling = 0;
	screen->colors.floor = 0;
	screen->map_height = 0;
	screen->map_width = 0;
	screen->map = NULL;
	screen->player = NULL;
	ft_init_keys(&screen->keys);
	screen->width = 0;
	screen->height = 0;
	screen->refresh = 1;
	screen->mlx = NULL;
	screen->win = NULL;
	screen->sprite_count = 0;
	screen->sprites = NULL;
	ft_init_image_data(&screen->sprite);
	ft_init_image_data(&screen->buf);
	i = 0;
	while (i < 4)
	{
		ft_init_image_data(&screen->walls[i]);
		++i;
	}
	screen->score = 0;
}

t_screen	*ft_new_screen(t_player *player)
{
	t_screen	*screen;

	screen = (t_screen*)malloc(sizeof(*screen));
	if (!screen)
		return (NULL);
	ft_init_screen(screen);
	screen->player = player;
	return (screen);
}
