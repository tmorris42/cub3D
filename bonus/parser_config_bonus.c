/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/03 23:27:39 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

int	ft_config_p(char **line_addr, t_map_data *map_data)
{
	char	*filename;
	int		i;
	char	*line;

	if (!line_addr)
		return (ft_error("Invalid next level configuration"));
	line = *line_addr;
	if (!line)
		return (ft_error("Invalid next level configuration"));
	if (line[0] != 'P' || line[1] != ' ')
		return (ft_error("Invalid indentifier"));
	if (map_data->next_level)
		return (ft_error("Duplicate next level definition"));
	line = &line[2];
	i = ft_skip_chars(line, " ", 0);
	if (ft_get_chr_index(' ', &line[i]) > -1)
		return (ft_error("Invalid filename in next level definition"));
	filename = ft_strdup(&line[i]);
	if (!filename)
		return (ft_error("Malloc error while configuring next level"));
	map_data->next_level = filename;
	return (1);
}
