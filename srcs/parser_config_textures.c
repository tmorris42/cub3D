/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config_textures.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 21:36:42 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	ft_config_nesw(char *line, t_map_data *map_data, int wall_index)
{
	char	*filename;
	int		i;

	if (!line || wall_index < 0 || wall_index > 3)
		return (ft_error("Invalid wall texture configuration"));
	if (map_data->textures[wall_index])
		return (ft_error("Duplicate wall texture definition"));
	i = ft_skip_chars(line, " ", 0);
	if (ft_get_chr_index(' ', &line[i]) > -1)
		return (ft_error("Invalid filename in wall texture definition"));
	filename = ft_strdup(&line[i]);
	if (!filename)
		return (ft_error("Malloc error while configuring wall texture"));
	map_data->textures[wall_index] = filename;
	return (1);
}

int	ft_config_n(char **line_addr, t_map_data *map_data)
{
	char	*line;

	if (!line_addr)
		return (ft_error("Error parsing north wall texture"));
	line = (*line_addr);
	if (ft_strncmp(line, "NO ", 3))
		return (ft_error("Illegal configuration command"));
	return (ft_config_nesw(&line[3], map_data, 0));
}

int	ft_config_e(char **line_addr, t_map_data *map_data)
{
	char	*line;

	if (!line_addr)
		return (ft_error("Error parsing north wall texture"));
	line = (*line_addr);
	if (ft_strncmp(line, "EA ", 3))
		return (ft_error("Illegal configuration command"));
	return (ft_config_nesw(&line[3], map_data, 1));
}

int	ft_config_w(char **line_addr, t_map_data *map_data)
{
	char	*line;

	if (!line_addr)
		return (ft_error("Error parsing north wall texture"));
	line = (*line_addr);
	if (ft_strncmp(line, "WE ", 3))
		return (ft_error("Illegal configuration command"));
	return (ft_config_nesw(&line[3], map_data, 3));
}

int	ft_config_s(char **line_addr, t_map_data *map_data)
{
	char	**arr;
	char	*line;

	line = *line_addr;
	if (line[0] == 'S' && line[1] == 'O' && line[2] == ' ')
		return (ft_config_nesw(&line[3], map_data, 2));
	arr = ft_split(line, ' ');
	if (!arr)
		return (ft_error("Malloc failed during split"));
	if (ft_strncmp(arr[0], "S", 2) || !arr[1] || arr[2])
	{
		ft_free_array(arr);
		return (ft_error("Invalid SO wall texture configuration"));
	}
	if (map_data->sprite)
	{
		ft_free_array(arr);
		return (ft_error("Duplicate sprite textures"));
	}
	map_data->sprite = ft_strdup(arr[1]);
	ft_free_array(arr);
	if (!(map_data->sprite))
		return (ft_error("Malloc error while configuring sprite texture"));
	return (1);
}
