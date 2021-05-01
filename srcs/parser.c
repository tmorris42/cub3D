/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 15:07:23 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

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
	if (!line_addr || !(*line_addr) || !(map_data))
		return (-1);
	line = *line_addr;
	if (line[0] == '\0')
		return (0);
	i = ft_get_chr_index(line[0], "RFCNEWS");
	if (i != -1 && (map_data->map_width || map_data->map_height))
		return (ft_error("Map is not the last item in the configuration file"));
	i = (*config[i + 1])(line_addr, map_data);
	return (i);
}

char		*ft_verify_data(t_map_data *map_data)
{
	int		i;

	if (!map_data)
		return ("Map incomplete or non-existant");
	i = 0;
	while (i < 4)
	{
		if (!map_data->textures[i])
			return ("Missing one or more wall textures");
		++i;
	}
	if (!map_data->sprite)
		return ("Missing sprite texture");
	if (map_data->res_width <= 0 || map_data->res_height <= 0)
		return ("Missing or invalid resolution");
	if (map_data->colors_set != 3)
		return ("Floor or ceiling color is missing");
	if (map_data->player_x == -1 || map_data->player_y == -1)
		return ("No starting location found");
	if (map_data->map_width <= 0 || map_data->map_height <= 0)
		return ("Map dimensions not properly detected");
	if (!map_data->player_facing_x && !map_data->player_facing_y)
		return ("No starting direction found");
	return (NULL);
}

int			check_map_zeroes(t_map_data *map); //

void		*ft_verify_all(t_map_data *data)
{
	char	*line;
	char	*msg;

	if (!data)
		return (NULL);
	if ((msg = ft_verify_data(data)))
		return (ft_free_map_error(data, msg));
	if (ft_convert_map_to_2d(data) == -1)
		return (ft_free_map_data(data));
	line = (char*)ft_calloc(data->map_width * data->map_height, sizeof(char));
	if (!line)
		return (ft_free_map_error(data, "Unable to allocate sufficient space"));
	if (check_map_zeroes(data) == -1)
		data = ft_free_map_error(data, "Map must be surrounded by walls");
	free(line);
	return (data);
}

t_map_data	*ft_parse_file(char *filename)
{
	int			fd;
	int			status;
	char		*line;
	t_map_data	*map_data;

	if (ft_strncmp(&filename[ft_strlen(filename) - 4], ".cub", 5))
		return (ft_free_map_error(NULL, "Map file must be a .cub file"));
	if (!(map_data = malloc(sizeof(*map_data))))
		return (ft_free_map_error(NULL, "Unable to allocate sufficient space"));
	ft_map_data_init(map_data);
	line = NULL;
	status = 0;
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (ft_free_map_error(map_data, "Unable to open map file"));
	while (status >= 0)
	{
		status = get_next_line(fd, &line);
		if (status < 0 || (map_data && ft_parse_line(&line, map_data) < 0))
			map_data = ft_free_map_data(map_data);
		free(line);
		line = NULL;
		if (status == 0)
			break ;
	}
	return (ft_verify_all(map_data));
}
