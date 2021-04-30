/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 18:27:39 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 22:25:43 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAYCAST_H
# define RAYCAST_H

typedef struct	s_segment
{
	t_pt	start;
	t_pt	end;
}				t_seg;

typedef struct	s_sprite_data
{
	t_pt	dim;
	t_d_pt	pos;
	double	screen_x;
	double	*buffer;
	int		*order;
	double	*dist;
	t_d_pt	plane;
	t_seg	draw;
	t_d_pt	transform;
}				t_sprite_data;

typedef struct	s_camera
{
	t_d_pt	plane;
	t_d_pt	ray_dir;
	t_pt	map;
	t_pt	step;
	t_d_pt	dist;
	t_d_pt	delta_dist;
}				t_camera;

typedef struct	s_wall_data
{
	int		num;
	double	x;
	double	dist;
	int		side_check;
}				t_wall_data;

typedef struct	s_texture
{
	int		x;
	int		y;
	double	pos;
}				t_texture;

void			ft_draw_col(t_screen *scr, t_sprite_data *d, t_pt *t, t_pt *p);
void			ft_render_sprite(t_screen *scr, t_sprite_data *data);
void			ft_draw_sprite(t_screen *screen, t_sprite_data *data, int *i);
int				ft_cast_sprites(t_screen *scr, t_sprite_data *data);
void			ft_sort_sprites(t_sprite_data *sprite_data, int amount);
void			ft_calc_transform(t_screen *screen, t_sprite_data *data);
int				ft_free_sprite_data(t_sprite_data *data);
void			ft_init_sprite_data(t_sprite_data *data);

#endif
