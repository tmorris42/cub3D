/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 14:18:30 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

void		ft_map_data_init(t_map_data *map)
{
	int		i;

	if (map)
	{
		map->colors_set = 0;
		map->floor = 0;
		map->ceil = 0;
		map->res_width = 0;
		map->res_height = 0;
		map->map_width = 0;
		map->map_height = 0;
		i = -1;
		while (++i < 4)
			map->textures[i] = NULL;
		map->sprite = NULL;
		map->sprite_count = 0;
		map->sprite_list = NULL;
		map->map_layout = NULL;
		map->map_grid = NULL;
		map->player_x = -1;
		map->player_y = -1;
		map->player_facing_x = 0;
		map->player_facing_y = 0;
	}
}

void		ft_free_int_array(int **arr, int y_max)
{
	int		i;

	if (!arr)
		return ;
	i = 0;
	while (i < y_max)
	{
		free(arr[i]);
		++i;
	}
	free(arr);
}

t_map_data	*ft_free_map_data(t_map_data *map)
{
	int		i;

	if (!map)
		return (NULL);
	i = -1;
	while (++i < 4)
	{
		free(map->textures[i]);
		map->textures[i] = NULL;
	}
	free(map->sprite);
	map->sprite = NULL;
	ft_lstclear(&(map->map_layout), &free);
	map->map_layout = NULL;
	ft_free_int_array(map->map_grid, map->map_height);
	map->map_grid = NULL;
	ft_lstclear(&map->sprite_list, &free);
	map->sprite_list = NULL;
	free(map);
	return (NULL);
}

int			ft_free_array(char **array)
{
	int		i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (-1);
}

int			ft_create_sprite(t_map_data *map, int i, int j)
{
	t_sprite	*temp_sprite;
	t_list		*temp_node;

	temp_sprite = malloc(sizeof(*temp_sprite));
	if (!temp_sprite)
		return (-1);
	temp_sprite->x = i * 1.0 + 0.5;
	temp_sprite->y = j * 1.0 + 0.5;
	temp_node = ft_lstnew(temp_sprite);
	if (!temp_node)
	{
		free(temp_sprite);
		return (-1);
	}
	ft_lstadd_front(&map->sprite_list, temp_node);
	map->sprite_count++;
	return (0);
}
