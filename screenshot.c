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

#include "header.h"

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
	ft_putint_fd(124, fd);
	ft_putint_fd(0, fd);
	ft_putchar_fd(26, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	return (1);
}

static int	ft_print_info(int fd, t_screen *screen)
{
	ft_putint_fd(12, fd);
	ft_puthex_pair_fd(screen->width, fd);
	ft_puthex_pair_fd(screen->height, fd);
	ft_putchar_fd(1, fd);
	ft_putchar_fd(0, fd);
	ft_putchar_fd(24, fd);
	ft_putchar_fd(0, fd);
	return (1);
}

static int	ft_print_pixels(int fd, t_screen *screen)
{
	int				max;
	char			c;
	int				x;
	int				y;
	unsigned int	pixel;

	max = screen->width * screen->height;
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
			x = (4 - ((max * 3) % 4)) * ((max * 3) % 4 != 0) + 1;
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
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
	close(fd);
	return (1);
}
