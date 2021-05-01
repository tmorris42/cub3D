/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 17:35:57 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int			ft_copy_map_line(t_map_data *map, int **grid, char *content, int j)
{
	int			i;

	if (!(grid[j] = (int*)ft_calloc(map->map_width, sizeof(**grid))))
		return (ft_error("Could not allocate sufficient space"));
	i = 0;
	while ((i < map->map_width) && content[i])
	{
		grid[j][i] = content[i];
		if (grid[j][i] == '2')
		{
			if (ft_create_sprite(map, i, j) == -1)
				return (ft_error("Could not allocate sufficient space"));
			grid[j][i] = '0';
		}
		++i;
	}
	return (0);
}

int			ft_convert_map_to_2d(t_map_data *map)
{
	int			**grid;
	t_list		*index;
	int			j;

	if (!map)
		return (-1);
	index = map->map_layout;
	grid = (int**)ft_calloc(map->map_height, sizeof(*grid));
	if (!grid)
		return (ft_error("Could not allocate sufficient space"));
	j = 0;
	while (j < map->map_height)
	{
		if (ft_copy_map_line(map, grid, (char*)index->content, j) == -1)
		{
			ft_free_int_array(grid, map->map_height);
			return (-1);
		}
		++j;
		index = index->next;
	}
	map->map_grid = grid;
	return (1);
}

static int	ft_check_surrounding(t_map_data *map, int x, int y)
{
	int		**arr;

	arr = map->map_grid;
	if (arr[y][x] == '0')
	{
		if (y == 0 || y == map->map_height - 1)
			return (-1);
		if (x == 0 || x == map->map_width - 1)
			return (-1);
		if (arr[y - 1][x] < '0' || arr[y + 1][x] < '0')
			return (-1);
		if (arr[y][x - 1] < '0' || arr[y][x + 1] < '0')
			return (-1);
		if (arr[y - 1][x - 1] < '0' || arr[y - 1][x + 1] < '0')
			return (-1);
		if (arr[y + 1][x - 1] < '0' || arr[y + 1][x + 1] < '0')
			return (-1);
	}
	return (0);
}

static void	ft_convert_map_to_int(t_map_data *map)
{
	int		x;
	int		y;
	int		**arr;

	arr = map->map_grid;
	y = 0;
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
			arr[y][x] -= '0';
			++x;
		}
		++y;
	}
}

int			ft_check_map_zeroes(t_map_data *map)
{
	int		x;
	int		y;

	if (!map)
		return (-1);
	y = 0;
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
			if (ft_check_surrounding(map, x, y) < 0)
				return (-1);
			++x;
		}
		++y;
	}
	ft_convert_map_to_int(map);
	return (0);
}
