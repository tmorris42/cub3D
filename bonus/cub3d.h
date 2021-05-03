/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 00:24:37 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 19:13:16 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <fcntl.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

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

# ifndef ERROR_FD
#  define ERROR_FD 2
# endif

typedef struct		s_point
{
	int		x;
	int		y;
}					t_pt;

typedef struct		s_double_point
{
	double x;
	double y;
}					t_d_pt;

typedef struct		s_sprite
{
	double	x;
	double	y;
}					t_sprite;

typedef struct		s_keys
{
	int		up;
	int		down;
	int		left;
	int		right;
	int		turn_right;
	int		turn_left;
}					t_keys;

typedef struct		s_map_data
{
	int				colors_set;
	unsigned int	floor;
	unsigned int	ceil;
	int				res_width;
	int				res_height;
	int				map_width;
	int				map_height;
	char			*textures[4];
	char			*sprite;
	int				sprite_count;
	t_list			*sprite_list;
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
	int			**map;
	int			map_width;
	int			map_height;
	t_player	*player;
	t_keys		keys;
	t_img_data	walls[4];
	t_img_data	sprite;
	int			sprite_count;
	t_sprite	*sprites;
	t_colors	colors;
	int			refresh;
}					t_screen;

t_map_data			*ft_free_map_data(t_map_data *map);
t_map_data			*ft_parse_file(char *filename);
int					ft_save(t_screen *screen, char *filename);
void				ft_pixel_put(t_img_data *img, int x, int y, int color);
unsigned int		ft_get_pixel_from_image(t_img_data *img, int x, int y);
int					ft_load_image(char *path, t_screen *scr, t_img_data *img);
t_screen			*ft_new_screen(t_player *player);
int					ft_close_screen(t_screen **screen_addr);
void				ft_free_int_array(int **arr, int y_max);
void				ft_close_on_error(t_screen *screen, char *msg);
int					ft_raycast(t_screen *screen);
int					ft_draw(t_screen *screen);
int					ft_redraw(t_screen *screen);
void				ft_rotate(double rad, t_screen *screen);
void				ft_move_relative(double x, double y, t_screen *screen);
int					ft_press_keys(int key, t_screen *screen);
int					ft_lift_keys(int key, t_screen *screen);
void				ft_free_map_exit(t_screen *scr, t_map_data *map, char *msg);
int					ft_error(char *msg);
void				ft_close_on_error(t_screen *screen, char *msg);
t_map_data			*ft_free_map_error(t_map_data *map, char *msg);
t_screen			*ft_load_screen(t_player *p, t_map_data *data, int save);
void				check_object_collision(t_screen *screen);

#endif
