#ifndef CUB3D_HEADER_H
# define CUB3D_HEADER_H
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "minilibx-linux/mlx.h"
# include "libft/libft.h"

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
	t_list	*map_layout;
	int		**map_grid;
	int		player_x;
	int		player_y;
	int		player_facing_x;
	int		player_facing_y;

}				t_map_data;


void		ft_free_map_data(t_map_data *map);
void		ft_print_map_data(t_map_data map);
t_map_data	*ft_parse_file(char *filename);

#endif
