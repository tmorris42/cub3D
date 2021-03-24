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

#include "cub3d.h"
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
		map->sprite_count = 0;
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
			map->res_height, map->map_width, map->map_height);
	i = -1;
	while (++i < 4)
		printf("\t%s\n", map->textures[i]);
	printf("Sprite:\n\t%s\n", map->sprite);
	printf("Number of Sprites = %d\n", map->sprite_count);
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

int			ft_get_chr_index(char c, char *str)
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

void		ft_set_player_start(t_map_data *map_data, char dir, int x)
{
	map_data->player_x = x;
	map_data->player_y = map_data->map_height;
	map_data->player_facing_x = 0;
	map_data->player_facing_y = 0;
	if (dir == 'W')
		map_data->player_facing_x = -1;
	else if (dir == 'N')
		map_data->player_facing_y = -1;
	if (dir == 'E')
		map_data->player_facing_x = 1;
	if (dir == 'S')
		map_data->player_facing_y = 1;
}

static int	ft_free_array(char **array)
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

static int	ft_config_other(char **line_addr, t_map_data *map_data)
{
	int		i;
	t_list	*new;

	i = 0;
	while ((*line_addr) && (*line_addr)[i])
	{
		if (ft_strchr("NESW", (*line_addr)[i]))
		{
			if (map_data->player_x != -1 || map_data->player_y != -1)
				return (-1); // error, duplicate player symobls
			ft_set_player_start(map_data, (*line_addr)[i], i);
			(*line_addr)[i] = '0';
		}
		else if (ft_strchr(" 012", (*line_addr)[i]) == NULL)
			return (-1);
		++i;
	}
	if (!(new = ft_lstnew((*line_addr))))
		return (-1);
	ft_lstadd_back(&(map_data->map_layout), new);
	map_data->map_width = ft_max(map_data->map_width, ft_strlen((*line_addr)));
	map_data->map_height += 1;
	*line_addr = NULL;
	return (1);
}

static int	ft_config_r(char **line_addr, t_map_data *map_data)
{
	char	**arr;
	char	*line;

	line = *line_addr;
	if (map_data->res_width != 0 || map_data->res_height != 0)
		return (-1); //error\n duplicate resolution
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "R", 2) || !arr[1] || !arr[2] || arr[3])
		return (ft_free_array(arr)); //error, invalid R configuration
	map_data->res_width = ft_atoi(arr[1]);
	map_data->res_height = ft_atoi(arr[2]);
	if (map_data->res_width <= 0 || map_data->res_height <= 0)
		return (ft_free_array(arr)); //error, invalid resolution
	ft_free_array(arr);
	return (1);
}

static int	ft_parse_rgb(char *line, unsigned int *rgb)
{
	char	**arr;
	int		i;
	int		temp;

	(*rgb) = 0;
	i = -1;
	while (line && line[++i])
	{
		if (!(ft_isdigit(line[i])) && line[i] != ',')
			return (-1); //invalid RGB value
	}
	if (!(arr = ft_split(line, ',')))
		return (-1);
	i = -1;
	while (arr[++i])
	{
		temp = ft_atoi(arr[i]);
		if (i > 2 || temp < 0 || temp > 255)
			return (ft_free_array(arr));
		(*rgb) = ((*rgb) << 8) + temp;
	}
	ft_free_array(arr);
	return (1);
}

static int	ft_config_f(char **line_addr, t_map_data *map_data)
{
	char			**arr;
	unsigned int	rgb_i;
	int				status;
	char			*line;

	line = *line_addr;
	if (map_data->colors_set & 1)
		return (-1); //error, floor already set.
	status = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "F", 2) || !arr[1] || arr[2])
		return (ft_free_array(arr)); //error, invalid NO configuration
	if ((status = ft_parse_rgb(arr[1], &rgb_i)) == -1)
		return (ft_free_array(arr)); //err, invalid rgb
	map_data->floor = rgb_i;
	map_data->colors_set += 1;
	ft_free_array(arr);
	return (1);
}

static int	ft_config_c(char **line_addr, t_map_data *map_data)
{
	char			**arr;
	unsigned int	rgb_i;
	int				status;
	char			*line;

	line = *line_addr;
	if (map_data->colors_set & 2)
		return (-1); //error, floor already set.
	rgb_i = 0;
	status = 0;
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "C", 2) || !arr[1] || arr[2])
		return (ft_free_array(arr)); //error, invalid NO configuration
	if ((status = ft_parse_rgb(arr[1], &rgb_i)) == -1)
		return (ft_free_array(arr)); //err, invalid rgb
	map_data->ceil = rgb_i;
	map_data->colors_set += 2;
	ft_free_array(arr);
	return (1);
}

static int	ft_config_nesw(char **line_addr, t_map_data *map_data, char *code)
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
	if (map_data->textures[index])
		return (-1); //error, texture already supplied
	arr = ft_split(*line_addr, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], code, 3) || !arr[1] || arr[2])
		return (ft_free_array(arr)); //error, invalid NO configuration
	filename = ft_strdup(arr[1]);
	map_data->textures[index] = filename;
	ft_free_array(arr);
	return (1);
}

static int	ft_config_n(char **line_addr, t_map_data *map_data)
{
	return (ft_config_nesw(line_addr, map_data, "NO"));
}

static int	ft_config_e(char **line_addr, t_map_data *map_data)
{
	return (ft_config_nesw(line_addr, map_data, "EA"));
}

static int	ft_config_w(char **line_addr, t_map_data *map_data)
{
	return (ft_config_nesw(line_addr, map_data, "WE"));
}

static int	ft_config_s(char **line_addr, t_map_data *map_data)
{
	char	**arr;
	char	*filename;
	char	*line;

	line = *line_addr;
	if (line[0] == 'S' && line[1] == 'O')
		return (ft_config_nesw(line_addr, map_data, "SO"));
	arr = ft_split(line, ' ');
	if (!arr)
		return (-1); //error msg split failed / error parsing map
	if (ft_strncmp(arr[0], "S", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (-1); //error, invalid NO configuration
	}
	filename = ft_strdup(arr[1]);
	map_data->sprite = filename;
	ft_free_array(arr);
	return (1);
}

static int	ft_parse_line(char **line_addr, t_map_data *map_data)
{
	int		i;
	int		(*config[8]) (char **map_line, t_map_data *map_data);
	char	*line;

	config[0] = ft_config_other;
	config[1] = ft_config_r;
	config[2] = ft_config_f;
	config[3] = ft_config_c;
	config[4] = ft_config_n;
	config[5] = ft_config_e;
	config[6] = ft_config_w;
	config[7] = ft_config_s;
	i = 0;
	if (!line_addr || !(*line_addr))
		return (-1);
	line = *line_addr;
	if (line[0] == '\0')
		return (0);
	i = ft_get_chr_index(line[0], "RFCNEWS");
	i = (*config[i + 1])(line_addr, map_data);
	if (i == -1)
		printf("PARSING ERROR\n"); //be more explicit
	return (i);
}

int		ft_copy_map_line(t_map_data *map, int **grid, char *content, int j)
{
	int			i;
	t_sprite	*temp_sprite;

	grid[j] = (int*)ft_calloc(map->map_width, sizeof(**grid));
	if (!(grid[j]))
		return (-1);
	i = 0;
	while ((i < map->map_width) && (content)[i])
	{
		grid[j][i] = content[i] - '0';
		if (grid[j][i] == 2)
		{
			temp_sprite = malloc(sizeof(*temp_sprite));
			if (!temp_sprite)
				return (-1);
			temp_sprite->x = i * 1.0 + 0.5;
			temp_sprite->y = j * 1.0 + 0.5;
			ft_lstadd_front(&map->sprite_list, ft_lstnew(temp_sprite));
			map->sprite_count++;
			grid[j][i] = 0;
		}
		++i;
	}
	return (0);
}

int		ft_convert_map_to_2d(t_map_data *map)
{
	int			**grid;
	t_list		*index;
	int			j;

	if (!map)
		return (-1);
	index = map->map_layout;
	grid = (int**)ft_calloc(map->map_height, sizeof(*grid));
	if (!grid)
		return (-1);
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

int			ft_check_map_leaks(t_map_data *map, char *paths, int x, int y)
{
	int		**arr;
	int		str_index;

	arr = map->map_grid;
	str_index = (map->map_width * y) + x;
	if (x >= map->map_width || y >= map->map_height || (arr[y][x] == ' ' - '0'))
		return (-1);
	if (paths[str_index] == 'V')
		return (0);
	paths[str_index] = 'V';
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

int			ft_verify_data(t_map_data *map_data)
{
	int		i;

	if (!map_data)
		return (-1);
	i = 0;
	while (i < 4)
	{
		if (!map_data->textures[i])
			return (-1); //error, missing texture information
		++i;
	}
	if (!map_data->sprite)
		return (-1); //error, missing sprite texture information
	if (map_data->res_width <= 0 || map_data->res_height <= 0)
		return (-1); //error, missing or illegal resolution
	if (map_data->colors_set != 3)
		return (-1); //error, floor or ceiling color is missing
	//verify the rest too
	return (1);
}

void		*ft_verify_all(t_map_data *data)
{
	char	*line;

	if (ft_verify_data(data) == -1)
	{
		perror("Error\nMissing or invalid map data detected");
		return (ft_free_map_data(data)); //error, missing or incorrect info
	}
	if (ft_convert_map_to_2d(data) == -1)
	{
		perror("Error\nCould not parse map data");
		return (ft_free_map_data(data));
	}
	line = (char*)ft_calloc(data->map_width * data->map_height, sizeof(char));
	if (!line)
	{
		perror("Error\nCould not allocate space");
		return (ft_free_map_data(data));
	}
	if (ft_check_map_leaks(data, line, data->player_x, data->player_y) < 0)
	{
		perror("Error\nMap must be surrounded by walls in all 8 directions");
		data = ft_free_map_data(data);
	}
	free(line);
	return (data);
}

t_map_data	*ft_parse_file(char *filename)
{
	int			fd;
	int			status;
	char		*line;
	t_map_data	*map_data;

	if (!(map_data = malloc(sizeof(*map_data))))
		return (NULL);
	ft_map_data_init(map_data);
	status = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (ft_free_map_data(map_data));
	while (status >= 0)
	{
		status = get_next_line(fd, &line);
		if (map_data && ft_parse_line(&line, map_data) < 0)
			map_data = ft_free_map_data(map_data);
		free(line);
		line = NULL;
		if (status == 0)
			break ;
	}
	return (ft_verify_all(map_data));
}
