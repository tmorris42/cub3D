/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/03/08 18:38:20 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
#include <errno.h>

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
		map->sprite_num = 0;
		map->sprite_list = 0;
		map->map_layout = NULL;
		map->map_grid = NULL;
		map->player_x = -1;
		map->player_y = -1;
		map->player_facing_x = -1;
		map->player_facing_y = -1;
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
	ft_lstclear(&(map->map_layout), &free);
	map->map_layout = NULL;
	ft_free_int_array(map->map_grid, map->map_height);
	map->map_grid = NULL;
	map->sprite = NULL;
	ft_lstclear(&map->sprite_list, &free);
	free(map);
	return (NULL);
}


void		ft_print_map_data(t_map_data *map)
{
	int		i;
	int		j;

	t_list	*index;
	printf("Resolution: %dx%d\nMap Dimensions: %dx%d\nTextures:\n", map->res_width,
			map->res_height,	map->map_width, map->map_height);
	i = -1;
	while (++i < 4)
		printf("\t%s\n", map->textures[i]);
	printf("Sprite:\n\t%s\n", map->sprite);
	printf("Sprite_num = %d\n", map->sprite_num);
	printf("Floor Color: %u\nCeiling Color: %u\n", map->floor, map->ceil);
	printf("Player Location: %d, %d, <%d, %d>\n", map->player_x, map->player_y, map->player_facing_x, map->player_facing_y);
	ft_printf("Map Layout:\n");
	index = map->map_layout;
	while (index)
	{
		printf("\t|%s|\n", (char*)index->content);
		index = index->next;
	}
	if (!map->map_grid)
		return ;
	printf("Map Grid:\n");
	j = 0;
	while (j < map->map_height)
	{
		i = 0;
		while (i < map->map_width)
		{
			printf("%d", map->map_grid[j][i]);
			++i;
		}
		printf("\n");
		++j;
	}
	printf("Done printing map data\n");
}

int	 		ft_get_chr_index(char c, char *str)
{
	int		i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == c)
			return (i);
		++i;
	}
	return (-1);
}

int			ft_get_direction_vector(char dir, int xy)
{
	if (xy == 0 && (dir == 'N' || dir == 'S'))
		return (0);
	if (xy == 1 && (dir == 'W' || dir == 'E'))
		return (0);
	if (dir == 'W' || dir == 'N')
		return (-1);
	if (dir == 'E' || dir == 'S')
		return (1);
	return (0);

}

static void	ft_free_array(char **array)
{
	int		i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

static int	ft_config_other(char *line, t_map_data *map_data)
{
	int		i;
	char	*mapline;
	t_list	*new;

	i = 0;
	while (line && line[i])
	{
		if (ft_strchr("NESW", line[i]))
		{
			// if player loc set ==> error
			if (map_data->player_x != -1 || map_data->player_y != -1)
				return (-1); // error, duplicate player symobls
			else
			{
				map_data->player_x = i;
				map_data->player_y = map_data->map_height;
				map_data->player_facing_x = ft_get_direction_vector(line[i], 0);
				map_data->player_facing_y = ft_get_direction_vector(line[i], 1);
				line[i] = '0';
			}
			// else, set it and facing  (remember height is not yet increased
		}
		else if (ft_strchr(" 012", line[i]) == NULL)
			return (-1);
		++i;
	}
	// if here, then only legal chars were found. //still have to check for mapleaks though
	mapline = ft_strdup(line);
	if (!mapline)
		return (-1);
	new = ft_lstnew(mapline);
	if (!new)
	{
		free(mapline);
		return (-1);
	}
	ft_lstadd_back(&(map_data->map_layout), new);
	map_data->map_width = ft_max(map_data->map_width, ft_strlen(mapline));
	map_data->map_height += 1;

	// Idea for parsing the map for leaks (lack of outer wall)
	// 		verify that a space is only adjacent to another space or a '1'
	// 			if it is next to a zero, that's probably a leak
	// 						unless the zero is just outside the map... hm
	// 							READ SUBJECT TO SEE IF THAT'S ALLOWED
	return (1);
}

static int	ft_config_r(char *line, t_map_data *map_data)
{
	char	**arr;

	if (map_data->res_width != 0 || map_data->res_height != 0)
		return (-1); //error\n duplicate resolution
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "R", 2) || !arr[1] || !arr[2] || arr[3])
	{
		ft_free_array(arr);
		return (-1); //error, invalid R configuration
	}
	map_data->res_width = ft_atoi(arr[1]);
	map_data->res_height = ft_atoi(arr[2]);
	if (map_data->res_width <= 0 || map_data->res_height <= 0)
	{
		ft_free_array(arr);
		return (-1); //error, invalid resolution
	}
	//ft_printf("\tR: %d, %d\n", ft_atoi(arr[1]), ft_atoi(arr[2]));
	// in reality, don't print, save to configuration struc, and verify that HEIGHT and WIDTH are both > 0
	ft_free_array(arr);
	return (1);
}

static int	ft_parse_rgb(char *line, unsigned int *rgb)
{
	char	**arr;
	int		i;
	int		temp;

	(*rgb) = 0;
	i = 0;
	while (line && line[i])
	{
		if (!(ft_isdigit(line[i])) && line[i] != ',')
			return (-1); //invalid RGB value
		++i;
	}
	arr = ft_split(line, ',');
	if (!arr)
		return (-1);
	i = 0;
	while (arr[i])
	{
		temp = ft_atoi(arr[i]);
		if (i > 2 || temp < 0 || temp > 255)
		{
			ft_free_array(arr);
			return (-1);
		}
		(*rgb) = ((*rgb) << 8) + temp;
		++i;
	}
	ft_free_array(arr);
	return (1);
}

static int	ft_config_f(char *line, t_map_data *map_data)
{
	char			**arr;
	unsigned int	rgb_i;
	int				status;

	if (map_data->colors_set & 1)
		return (-1); //error, floor already set.
	rgb_i = 0;
	status = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "F", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	status = ft_parse_rgb(arr[1], &rgb_i);
	if (status == -1)
	{
		ft_free_array(arr);
		return (-1); //err, invalid rgb
	}
	//ft_printf("\t%s: %s\n", code, filename);
	map_data->floor = rgb_i;
	// in reality, don't print, save to configuration struc, verify filename if necc.
	map_data->colors_set += 1;
	ft_free_array(arr);
	return (1);
}

static int	ft_config_c(char *line, t_map_data *map_data)
{
	char			**arr;
	unsigned int	rgb_i;
	int				status;

	if (map_data->colors_set & 2)
		return (-1); //error, floor already set.
	rgb_i = 0;
	status = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "C", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	status = ft_parse_rgb(arr[1], &rgb_i);
	if (status == -1)
	{
		ft_free_array(arr);
		return (-1); //err, invalid rgb
	}
	//ft_printf("\t%s: %s\n", code, filename);
	map_data->ceil = rgb_i;
	// in reality, don't print, save to configuration struc, verify filename if necc.
	map_data->colors_set += 2;
	ft_free_array(arr);
	return (1);
}

static int ft_config_nesw(char *line, t_map_data *map_data, char *code)
{
	char	**arr;
	char	*filename;
	int		index;

	if (code[0] == 'N' && code[1] == 'O')
		index = 0;
	else if (code[0] == 'E' && code[1] == 'A')
		index = 1;
	else if (code[0] == 'S' && code[1] == 'O')
		index = 2;
	else if (code[0] == 'W' && code[1] == 'E')
		index = 3;
	else
		return (-1);


	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], code, 3) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	filename = ft_strdup(arr[1]);
	//ft_printf("\t%s: %s\n", code, filename);
	map_data->textures[index] = filename;
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_config_n(char *line, t_map_data *map_data)
{
	return (ft_config_nesw(line, map_data, "NO"));
}

static int	ft_config_e(char *line, t_map_data *map_data)
{
	return (ft_config_nesw(line, map_data, "EA"));
}

static int	ft_config_w(char *line, t_map_data *map_data)
{
	return (ft_config_nesw(line, map_data, "WE"));
}

static int	ft_config_s(char *line, t_map_data *map_data)
{
	char	**arr;
	char	*filename;
	
	if (line[0] == 'S' && line[1] == 'O')
		return (ft_config_nesw(line, map_data, "SO"));
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "S", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	filename = ft_strdup(arr[1]);
	//ft_printf("\tS: %s\n", filename);
	map_data->sprite = filename;
	// in reality, don't print, save to configuration struc, verify filename if necc.
	ft_free_array(arr);
	return (1);
}

static int	ft_parse_line(char *line, t_map_data *map_data)
{
	int		i;
	int		(*config[8]) (char *map_line, t_map_data *map_data);
	int		parsing_map;

	config[0] = ft_config_other;
	config[1] = ft_config_r;
	config[2] = ft_config_f;
	config[3] = ft_config_c;
	config[4] = ft_config_n;
	config[5] = ft_config_e;
	config[6] = ft_config_w;
	config[7] = ft_config_s;

	parsing_map = 0;
	i = 0;
	if (!line || (parsing_map && line[0] == '\0'))
		return (-1);
	else if (line[0] == '\0')
		return (0);
	// check to make sure you don't seg fault by assuming len of string
	i = ft_get_chr_index(line[0], "RFCNEWS");
	i = (*config[i + 1]) (line, map_data);
	if (i == -1)
	{
		ft_printf("PARSING ERROR\n");
	}
	// if i == 0, assume that it was an empty line. 0 means nothing illegal, but not useful
	// if i == 1, something was found to be legal and useful
	//
	// in config_other, first check if it is an empty line, which is legal if not in map
	// then check if it is a map line, which is legal if in map
	// otherwise, return -1
	//
	// when parsing map lines, save each line into a linked lst
	// 		set a variable in the struct (map_width?) to be the maximum foudn value as you go
	// 		once true, max map_width is known, we can convert the linked list into a 2d array
	// 		using malloc, height = number of nodes, width = max_width
	//
	//
	
	return (i);
}

int		ft_convert_map_to_2d(t_map_data *map)
{
	int		**grid;
	t_list	*index;
	char	*content;
	int		i;
	int		j;

	if (!map)
		return (-1);
	index = map->map_layout;
	grid = (int**)ft_calloc(map->map_height, sizeof(*grid));
	if (!grid)
		return (-1);
	j = 0;
	while (j < map->map_height)
	{
		grid[j] = (int*)ft_calloc(map->map_width, sizeof(**grid));
		if (!(grid[j]))
		{
			ft_free_int_array(grid, map->map_height);
			return (-1);
		}
		i = 0;
		content = (char*)index->content;
		while ((i < map->map_width) && (content)[i])
		{
			grid[j][i] = content[i] - '0';
			//testing sprite
			if (grid[j][i] == 2)
			{
				t_sprite *temp_sprite;
				temp_sprite = malloc(sizeof(*temp_sprite));
				temp_sprite->x = i * 1.0 + 0.5;
				temp_sprite->y = j * 1.0 + 0.5;
				if (!temp_sprite)
					return (-1); //is this sufficient?
				ft_lstadd_front(&map->sprite_list, ft_lstnew(temp_sprite)); 
				printf("found a sprite during parsing\n");
				map->sprite_num++;
				grid[j][i] = 0;
			}
			++i;
		}
		++j;
		index = index->next;
	}
	map->map_grid = grid;
	return (1);
}

int			ft_check_map_leaks(t_map_data *map, char *paths, int x, int y)
{
	int		**arr;
	int		max_x;
	int		max_y;
	int		str_index;

	arr = map->map_grid;
	max_x = map->map_width;
	max_y = map->map_height;
	str_index = (max_x * y) + x;
	if (x >= max_x || y >= max_y)
		return (-1);
	if (arr[y][x] == ' ' - '0')
		return (-1);
	if (paths[str_index] == 'V')
		return (0);
	paths[str_index] = 'V';
//	ft_printf("arr[%d][%d] == %d\n", y, x, arr[y][x]);
	if (arr[y][x] == 1)
		return (1);
	if (ft_check_map_leaks(map, paths, x, y + 1) == -1)
		return (-1);
	if (ft_check_map_leaks(map, paths, x, y - 1) == -1)
		return (-1);
	if (ft_check_map_leaks(map, paths, x + 1, y) == -1)
		return (-1);
	if (ft_check_map_leaks(map, paths, x - 1, y) == -1)
		return (-1);
	return (0);
}

t_map_data	*ft_parse_file(char *filename)
{
	int		fd;
	int		status;
	char	*line;
	t_map_data *map_data;

	map_data = malloc(sizeof(*map_data));
	if (!map_data)
		return (NULL);
	ft_map_data_init(map_data);
	status = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_free_map_data(map_data);
		return (NULL); //free map_data
	}
	while (status >= 0)
	{
		status = get_next_line(fd, &line);
		if (map_data && ft_parse_line(line, map_data) < 0)
		{
			ft_free_map_data(map_data);
			map_data = NULL;
		}
		free(line);
		line = NULL;
		if (status == 0)	
			break ;
	}

	if (ft_convert_map_to_2d(map_data) == -1)
	{
		perror("Error\nCould not parse map data");
		ft_free_map_data(map_data);
		return (NULL);
	}
//	ft_printf("TESTING INDEXING:\n\n\t%c\n", map_data->map_layout[2][3]);
	line = (char*)ft_calloc(map_data->map_width * map_data->map_height, sizeof(char));
	if (!line)
	{
		perror("Error\nCould not allocate space");
		ft_free_map_data(map_data);
		return (NULL);
	}
	if (ft_check_map_leaks(map_data, line, map_data->player_x, map_data->player_y) < 0)
	{
		errno = EINVAL;
		ft_printf("Returned -1!! Illegal map.\n");
		perror("Error\nMap must be surrounded by walls in all 8 directions");
		ft_free_map_data(map_data);
		map_data = NULL;
	}
	free(line);
	ft_print_map_data(map_data);
	return (map_data);
}
/*
int		main(void)
{
	t_map_data	*map;
	map = ft_parse_file("illegalmap.cub");

	//ft_printf("MAP DATA\n");
	//ft_print_map_data(map);
	ft_printf("parse result pointer: %p\n", map);
	if (map)
		ft_free_map_data(map);

	return (0);
}
*/
