#include <unistd.h>
#include <fcntl.h>

#include "libft/libft.h"

typedef struct	s_map_data
{
	unsigned int	floor;
	unsigned int	ceil;
	int		res_width;
	int		res_height;
	int		map_width;
	int		map_height;
	char	*textures[4];
	char	*sprite;
//	t_map	*map_layout;
	int		player_x;
	int		player_y;
	double	player_facing;

}				t_map_data;

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
	map->player_x = -1;
	map->player_y = -1;
	map->player_facing = -1;
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
	map->sprite = NULL;
	//free(map)		// right now map is not malloc'd but if that changes
}


void		ft_print_map_data(t_map_data map)
{
	int		i;
	ft_printf("Resolution: %dx%d\nMap Dimensions: %dx%d\nTextures:\n", map.res_width,
			map.res_height,	map.map_width, map.map_height);
	i = -1;
	while (++i < 4)
		ft_printf("\t%s\n", map.textures[i]);
	ft_printf("Sprite:\n\t%s\n", map.sprite);
	ft_printf("Floor Color: %u\nCeiling Color: %u\n", map.floor, map.ceil);
	ft_printf("Player Location: %d, %d,  %d\n", map.player_x, map.player_y, map.player_facing);
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
	// Idea for parsing the map for leaks (lack of outer wall)
	// 		verify that a space is only adjacent to another space or a '1'
	// 			if it is next to a zero, that's probably a leak
	// 						unless the zero is just outside the map... hm
	// 							READ SUBJECT TO SEE IF THAT'S ALLOWED
	return (0);
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

	config[0] = ft_config_other;
	config[1] = ft_config_r;
	config[2] = ft_config_f;
	config[3] = ft_config_c;
	config[4] = ft_config_n;
	config[5] = ft_config_e;
	config[6] = ft_config_w;
	config[7] = ft_config_s;

	i = 0;
	if (!line || line[0] == '\0')
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

int			ft_parse_file(char *filename)
{
	int		fd;
	int		status;
	char	*line;
	t_map_data map_data;

	ft_map_data_init(&map_data);
	status = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	while (status >= 0)
	{
		status = get_next_line(fd, &line);
		if (ft_parse_line(line, &map_data) <= 0)
			ft_printf("%s\n", line);
		free(line);
		line = NULL;
		if (status == 0)	
			break ;
	}
	ft_printf("MAP DATA\n");
	ft_print_map_data(map_data);
	ft_free_map_data(&map_data);
	return (0);
}

int		main(void)
{
	ft_parse_file("example.cub");
	return (0);
}
