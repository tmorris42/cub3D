/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/16 18:05:52 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 23:07:50 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

int				ft_load_image(char *path, t_screen *screen, t_img_data *image)
{
	image->img = mlx_xpm_file_to_image(screen->mlx, path, &(image->width),
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
