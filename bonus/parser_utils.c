/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 18:41:50 by tmorris           #+#    #+#             */
/*   Updated: 2021/04/30 18:50:33 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "parser.h"

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

int			ft_islegal_char(char c, char *charset)
{
	int		i;

	i = ft_strlen(charset) - 1;
	while (i >= 0)
	{
		if (c == charset[i])
			return (1);
		--i;
	}
	return (0);
}

int			ft_has_only_legal_chars(char *str, char *charset)
{
	int		i;

	i = 0;
	while (str && str[i])
	{
		if (!(ft_islegal_char(str[i], charset)))
			return (0);
		++i;
	}
	return (1);
}

int			ft_skip_chars(char *str, char *charset, int i)
{
	if (i < 0 || !str || !charset)
		return (i);
	while (str[i] && ft_islegal_char(str[i], charset))
		++i;
	return (i);
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
