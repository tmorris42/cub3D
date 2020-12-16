#include "header.h"

void		ft_map_data_init(t_map_data *map)
{
	int		i;
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
	map->map_layout = NULL;
	map->map_grid = NULL;
	map->player_x = -1;
	map->player_y = -1;
	map->player_facing_x = -1;
	map->player_facing_y = -1;
}

void		ft_free_int_array(int **arr, int y_max)
{
	int		i;

	i = 0;
	if (!arr)
		return ;
	while (i < y_max)
	{
		free(arr[i]);
		++i;
	}
	free(arr);

}

void		ft_free_map_data(t_map_data *map)
{
	int		i;
	
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
	free(map);		// right now map is not malloc'd but if that changes
}


void		ft_print_map_data(t_map_data map)
{
	int		i;
	int		j;

	t_list	*index;
	ft_printf("Resolution: %dx%d\nMap Dimensions: %dx%d\nTextures:\n", map.res_width,
			map.res_height,	map.map_width, map.map_height);
	i = -1;
	while (++i < 4)
		ft_printf("\t%s\n", map.textures[i]);
	ft_printf("Sprite:\n\t%s\n", map.sprite);
	ft_printf("Floor Color: %u\nCeiling Color: %u\n", map.floor, map.ceil);
	ft_printf("Player Location: %d, %d, <%d, %d>\n", map.player_x, map.player_y, map.player_facing_x, map.player_facing_y);
	ft_printf("Map Layout:\n");
	index = map.map_layout;
	while (index)
	{
		ft_printf("\t|%s|\n", index->content);
		index = index->next;
	}
	if (!map.map_grid)
		return ;
	ft_printf("Map Grid:\n");
	j = 0;
	while (j < map.map_height)
	{
		i = 0;
		while (i < map.map_width)
		{
			ft_printf("%c", map.map_grid[j][i] + '0');
			++i;
		}
		ft_printf("\n");
		++j;
	}
}

int	 		ft_get_chr_index(char c, char *str)
{
	int		i;

	i = 0;
	while (str[i])
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
	ft_free_array(arr);
	return (1);
}

static int	ft_config_c(char *line, t_map_data *map_data)
{
	char			**arr;
	unsigned int	rgb_i;
	int				status;

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
			++i;
		}
		++j;
		index = index->next;
	}
	map->map_grid = grid;
	return (1);
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
		if (ft_parse_line(line, map_data) < 0)
			ft_printf("%s\n", line);
		free(line);
		line = NULL;
		if (status == 0)	
			break ;
	}

	if (!(ft_convert_map_to_2d(map_data)))
	{
		ft_free_map_data(map_data);
		return (NULL);
	}
//	ft_printf("TESTING INDEXING:\n\n\t%c\n", map_data->map_layout[2][3]);
	return (map_data);
}
/*
int		main(void)
{
	t_map_data	*map;
	map = ft_parse_file("example.cub");

	ft_printf("MAP DATA\n");
	ft_print_map_data(*map);
	ft_free_map_data(map);

	return (0);
}
*/
