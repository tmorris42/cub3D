/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 15:39:10 by tmorris          ###   ########.fr       */
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
#include <stdio.h> //
int			check_map_zeroes(t_map_data *map)
{
	int		**arr;
	int		x;
	int		y;

	if (!map)
		return (-1);
	y = 0;
	arr = map->map_grid;
	printf("Map dimensiones = %d, %d\n", map->map_width, map->map_height);
	while (y < map->map_height)
	{
		x = 0;
		while (x < map->map_width)
		{
//			printf("X=%d, Y=%d, Value=%d\n", x, y, arr[y][x]);//
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
			++x;
		}
		++y;
	}
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
	return (0);
}

int			ft_check_map_void(t_map_data *map, char *paths, int x, int y)
{
	int		**arr;
	int		str_index;
	int		i;
	int		j;

	arr = map->map_grid;
	if (x < 0 || y < 0)
		return (-1);
	str_index = (map->map_width * y) + x;
	if (x >= map->map_width || y >= map->map_height || (arr[y][x] == ' ' - '0'))
		return (-1);
	if (paths[str_index] == 'V')
		return (0);
	paths[str_index] = 'V';
	if (arr[y][x] == 1)
		return (1);
	i = -2;
	while (++i < 2)
	{
		j = -2;
		while (++j < 2)
			if ((i || j) && (ft_check_map_void(map, paths, x + i, y + j) == -1))
				return (-1);
	}
	return (0);
}
