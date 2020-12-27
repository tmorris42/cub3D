/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 20:00:44 by tmorris           #+#    #+#             */
/*   Updated: 2020/12/27 17:04:46 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	ft_putint_fd(int x, int fd)
{
//	unsigned char	byte;

//	byte = 0;
//	byte = (unsigned char)x;
	write(fd, &x, sizeof(x));
	return (1);
}

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
	int		i;
	int		max;
	char	c;
	int		x;
	int		y;
	unsigned int	pixel;

	max = screen->width * screen->height;
	i = 0;
	x = 0;
	y = 0;
	while (i < max)
	{
		pixel = ft_get_pixel_from_image(&screen->buf, x, y);
		if (i % 2 == 0)
			c = 0;
		else
			c = 255;
//			ft_putint_fd(0, fd);
//		else
//			ft_putint_fd(255, fd);
		ft_putchar_fd(c, fd);
		ft_putchar_fd(c, fd);
		ft_putchar_fd(c, fd);
		++x;
		if (x >= screen->width)
		{
			x = 0;
			++y;
		}
		++i;
	}
	i = (4 - ((max * 3) % 4)) * ((max * 3) % 4 != 0);
	while (i > 0)
	{
		ft_putchar_fd(0, fd);
		--i;
	}
	return (1);
}

void		ft_print_excess(int fd)
{
	ft_putchar_fd(0, fd);
	ft_putchar_fd(0, fd);
}

int		ft_save(t_screen *screen, char *filename)
{
	int		fd;
	//open file
	fd = open(filename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
//		error and quit
		ft_printf("open error\n");
		return (-1);
	}
	ft_printf("SAVING to '%s'\n", "test.bmp");
	ft_print_header(fd);
	ft_print_info(fd, screen);
	ft_print_pixels(fd, screen);
	ft_print_excess(fd);
	close(fd);
	return (1);
}
