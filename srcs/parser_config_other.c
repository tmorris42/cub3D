/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config_other.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 21:33:20 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int		ft_config_other(char **line_addr, t_map_data *map_data)
{
	int		i;
	t_list	*new;

	i = 0;
	while ((*line_addr) && (*line_addr)[i])
	{
		if (ft_strchr("NESW", (*line_addr)[i]))
		{
			if (map_data->player_x != -1 || map_data->player_y != -1)
				return (ft_error("Multiple player symbols found"));
			ft_set_player_start(map_data, (*line_addr)[i], i);
			(*line_addr)[i] = '0';
		}
		else if (ft_strchr(" 012", (*line_addr)[i]) == NULL)
			return (ft_error("Illegal variable or illegal character in map"));
		++i;
	}
	if (!(new = ft_lstnew((*line_addr))))
		return (ft_error("Malloc failed during map parsing"));
	ft_lstadd_back(&(map_data->map_layout), new);
	map_data->map_width = ft_max(map_data->map_width, ft_strlen((*line_addr)));
	map_data->map_height += 1;
	*line_addr = NULL;
	return (1);
}

int		ft_config_r(char **line_addr, t_map_data *map_data)
{
	char	*line;
	int		i;

	if (!line_addr || !map_data)
		return (ft_error("Invalid line or map data"));
	line = *line_addr;
	if (map_data->res_width != 0 || map_data->res_height != 0)
		return (ft_error("Duplicate resolution configuration"));
	if (ft_strncmp(line, "R ", 2))
		return (ft_error("Invalid resolution configuration"));
	i = 2;
	if (!ft_has_only_legal_chars(&line[i], "0123456789 "))
		return (ft_error("Invalid characters in resolution configuration"));
	i = ft_skip_chars(line, " ", 2);
	map_data->res_width = ft_atoi(&line[i]);
	i = ft_skip_chars(line, "0123456789", i);
	i = ft_skip_chars(line, " ", i);
	map_data->res_height = ft_atoi(&line[i]);
	i = ft_skip_chars(line, "0123456789", i);
	i = ft_skip_chars(line, " ", i);
	if (line_addr && line && line[i])
		return (ft_error("Invalid resolution configuration"));
	return (1);
}

int		ft_parse_rgb(char *line, unsigned int *rgb)
{
	int		i;
	int		color_channel;
	int		colors_found;

	colors_found = 0;
	color_channel = 0;
	if (!ft_has_only_legal_chars(line, "0123456789, "))
		return (ft_error("Invalid character(s) in RGB configuration"));
	i = ft_skip_chars(line, " ", 0);
	while (line && line[i] && ft_isdigit(line[i]))
	{
		color_channel = ft_atoi(&line[i]);
		if (color_channel < 0 || color_channel > 255)
			return (ft_error("Invalid RGB value"));
		(*rgb) = ((*rgb) << 8) + color_channel;
		colors_found += 1;
		i = ft_skip_chars(line, "0123456789", i);
		if (colors_found != 3 && line[i] == ',')
			++i;
	}
	if (line[i] || colors_found != 3)
		return (ft_error("Invalid RGB configuration"));
	return (1);
}

int		ft_config_f(char **line_addr, t_map_data *map_data)
{
	unsigned int	rgb_i;
	int				status;
	char			*line;

	line = *line_addr;
	if (map_data->colors_set & 1)
		return (ft_error("Duplicate floor color configuration"));
	rgb_i = 0;
	if (ft_strncmp(line, "F ", 2))
		return (ft_error("Incomplete floor color configuration"));
	status = 0;
	if ((status = ft_parse_rgb(&line[2], &rgb_i)) == -1)
		return (-1);
	map_data->floor = rgb_i;
	map_data->colors_set += 1;
	return (1);
}

int		ft_config_c(char **line_addr, t_map_data *map_data)
{
	unsigned int	rgb_i;
	int				status;
	char			*line;

	line = *line_addr;
	if (map_data->colors_set & 2)
		return (ft_error("Duplicate ceiling color configuration"));
	rgb_i = 0;
	if (ft_strncmp(line, "C ", 2))
		return (ft_error("Incomplete ceiling color configuration"));
	status = 0;
	if ((status = ft_parse_rgb(&line[2], &rgb_i)) == -1)
		return (-1);
	map_data->ceil = rgb_i;
	map_data->colors_set += 2;
	return (1);
}
