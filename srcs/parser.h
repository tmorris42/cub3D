/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorris <tmorris@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 20:07:57 by tmorris           #+#    #+#             */
/*   Updated: 2021/05/01 17:28:24 by tmorris          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

int			ft_config_other(char **line_addr, t_map_data *map_data);
int			ft_config_r(char **line_addr, t_map_data *map_data);
int			ft_config_f(char **line_addr, t_map_data *map_data);
int			ft_config_c(char **line_addr, t_map_data *map_data);
int			ft_config_n(char **line_addr, t_map_data *map_data);
int			ft_config_e(char **line_addr, t_map_data *map_data);
int			ft_config_w(char **line_addr, t_map_data *map_data);
int			ft_config_s(char **line_addr, t_map_data *map_data);
int			ft_get_chr_index(char c, char *str);
int			ft_islegal_char(char c, char *charset);
int			ft_has_only_legal_chars(char *str, char *charset);
int			ft_skip_chars(char *str, char *charset, int i);
void		ft_set_player_start(t_map_data *map_data, char dir, int x);
void		ft_map_data_init(t_map_data *map);
void		ft_free_int_array(int **arr, int y_max);
t_map_data	*ft_free_map_data(t_map_data *map);
int			ft_free_array(char **array);
int			ft_create_sprite(t_map_data *map, int i, int j);
int			ft_convert_map_to_2d(t_map_data *map);
int			ft_check_map_zeroes(t_map_data *map);
#endif
