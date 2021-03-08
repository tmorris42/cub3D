/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 00:24:37 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 23:08:38 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_HEADER_H
# define CUB3D_HEADER_H
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include "minilibx-linux/mlx.h"
# include "libft/libft.h"

# define K_ESCAPE	65307
# define K_LEFT		65361
# define K_UP		65362
# define K_RIGHT	65363
# define K_DOWN		65364
# define K_W		119
# define K_A		97
# define K_S		115
# define K_D		100

# define RED		0xff0000
# define GREEN		0x00ff00
# define BLUE		0x0000ff
# define DARKGRAY	1315860
# define LIGHTGRAY	13158600

# define TRUE		1
# define FALSE		0

typedef struct		s_sprite
{
	double	x;
	double	y;
//	int	x;
//	int y;
}					t_sprite;

typedef struct		s_map_data
{
	unsigned int	floor;
	unsigned int	ceil;
	int				res_width;
	int				res_height;
	int				map_width;
	int				map_height;
	char			*textures[4];
	char			*sprite;
	t_sprite		sprites[1]; // should be dynamic, not limited to 1
	t_list			*map_layout;
	int				**map_grid;
	int				player_x;
	int				player_y;
	int				player_facing_x;
	int				player_facing_y;

}					t_map_data;

typedef struct		s_player
{
	double	pos_x;
	double	pos_y;
	double	rot_x;
	double	rot_y;
}					t_player;

typedef struct		s_image_data
{
	void	*img;
	int		width;
	int		height;
	char	*addr;
	int		bpp;
	int		len;
	int		endian;
}					t_img_data;

typedef struct		s_colors
{
	int		floor;
	int		ceiling;
}					t_colors;

typedef struct		s_screen
{
	void		*mlx;
	void		*win;
	t_img_data	buf;
	int			width;
	int			height;
	int			**map_data; //rename to map, please
	int			map_width;
	int			map_height;
	t_player	*player;
	t_img_data	walls[4];
	t_img_data	sprite;
	t_sprite	sprites[1]; //obviously should be a pointer to a dynamic malloc array (or linked lists)
	t_colors	colors;
	int			refresh;
}					t_screen;

t_map_data			*ft_free_map_data(t_map_data *map);
void				ft_print_map_data(t_map_data *map);
t_map_data			*ft_parse_file(char *filename);
int					ft_save(t_screen *screen, char *filename);
void				ft_pixel_put(t_img_data *img, int x, int y, int color);
unsigned int		ft_get_pixel_from_image(t_img_data *img, int x, int y);
t_screen			*ft_new_screen(t_player *player);
int					ft_close_screen(t_screen **screen_addr);
void				ft_free_int_array(int **arr, int y_max);
void				ft_close_on_error(t_screen *screen, char *msg);
void				ft_raycast(t_screen *screen);
int					ft_draw(t_screen *screen);
int					ft_redraw(t_screen *screen);

#endif
