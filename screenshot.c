/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 20:00:44 by tmorris           #+#    #+#             */
/*   Updated: 2020/12/27 19:01:34 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	ft_puthex_pair_fd(int x, int fd)
{
	if (x < 256)
	{
		ft_putchar_fd(x, fd);
		ft_putchar_fd(0, fd);
	}
	else
	{
		ft_putchar_fd(x % 256, fd);
		ft_putchar_fd(x / 256, fd);
	}
}

static int	ft_print_header(int fd)
{
	ft_putstr_fd("BM", fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putchar_fd(54, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	return (1);
}

static int	ft_print_info(int fd, t_screen *screen)
{
	ft_putchar_fd(40, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_puthex_pair_fd(screen->width, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_puthex_pair_fd(screen->height, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(1, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(24, fd);
	ft_putchar_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	ft_putint_fd(0, fd);
	return (1);
}

static int	ft_print_pixels(int fd, t_screen *screen)
{
	int				x;
	int				y;
	unsigned int	pixel;
	int				padding;

	padding = ((screen->width * 3) % 4);
	x = 0;
	y = screen->height - 1;
	while (y >= 0)
	{
		pixel = ft_get_pixel_from_image(&screen->buf, x, y);
		ft_putchar_fd((char)(pixel & 0xFF), fd);
		ft_putchar_fd((char)((pixel >> 8) & 0xFF), fd);
		ft_putchar_fd((char)((pixel >> 16) & 0xFF), fd);
		x++;
		if (x >= screen->width)
		{
			x = (4 - padding) * (padding != 0) + 1;
			while (--x > 0)
				ft_putchar_fd(0, fd);
			--y;
		}
	}
	return (1);
}

int			ft_save(t_screen *screen, char *filename)
{
	int		fd;

	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
		return (-1);
	ft_printf("SAVING to '%s'\n", filename);
	ft_print_header(fd);
	ft_print_info(fd, screen);
	ft_print_pixels(fd, screen);
	close(fd);
	return (1);
}
