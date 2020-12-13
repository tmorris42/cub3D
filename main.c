#include <stdio.h> //remove this
#include <stdlib.h> //for exit() EXIT_SUCCESS, EXIT_FAILURE
#include "header.h"
#include "libft/libft.h"

#define WIDTH	300
#define HEIGHT	200

#define K_ESCAPE	65307
#define K_LEFT		65361
#define K_UP		65362
#define K_RIGHT		65363
#define K_DOWN		65364
#define K_W			119
#define K_A			97
#define K_S			115
#define K_D			100

typedef struct		s_player
{
	int		x;
	int		y;
	double facing; //radians
}					t_player;

typedef struct		s_screen
{
	void	*mlx;
	void	*win;
	char	map_data[5][5];
	t_player	*player;
}					t_screen;

int		ft_get_color(int r, int g, int b, int alpha)
{
	return ((alpha << 24) + (r << 16) + (g << 8) + (b));
}

int		ft_draw(t_screen *screen)
{
	int		x;
	int		y;
	char	print[6];

	mlx_clear_window(screen->mlx, screen->win);
	print[5] = '\0';
	y = 0;
	while (y < 5)
	{
		x = 0;
		while (x < 5)
		{
//			ft_printf("map_data[%d,%d] = %c\n", x, y, (screen->map_data)[y][x]);
			if (screen->player->x == x && screen->player->y == y)
				print[x] = '*';
			else
				print[x] = screen->map_data[y][x];
			++x;
		}
//		ft_printf("%s\n", print);
		mlx_string_put(screen->mlx, screen->win, 10, 10 + 10 * y, 255, print);
		++y;
	}
	mlx_pixel_put(screen->mlx, screen->win, 10, 10, ft_get_color(255,0,0,0));
//	mlx_string_put(screen->mlx, screen->win, 10, 10, 255, "This is Blue");
//	mlx_string_put(screen->mlx, screen->win, 10, 20, get_color(0, 255, 0), "This is Green");
//	mlx_string_put(screen->mlx, screen->win, 10, 40, get_color(255, 0, 0), "This is Red");
	return (1);
}

int		ft_close_screen(void *win, t_screen *screen)
{
	if (screen->mlx && screen->win)
		mlx_destroy_window(screen->mlx, screen->win);
	if (screen->mlx)
		mlx_destroy_display(screen->mlx);
	free(screen->mlx);
	exit(EXIT_SUCCESS);
	return (1);
}

int		ft_parse_keys(int	key, t_screen *screen)
{
	if (key == K_ESCAPE)
		ft_close_screen(screen->win, screen);
	else if (key == K_UP)
		screen->player->y -= 1;
	else if (key == K_DOWN)
		screen->player->y += 1;
	else if (key == K_LEFT)	//should be turn left
		screen->player->x -= 1;
	else if (key == K_RIGHT) //should be turn right
		screen->player->x += 1;
	else
		printf(" : %d\n", key);
	return (1);
}

t_screen	set_map(t_screen screen)
{
    screen.map_data[0][0] = '1';
    screen.map_data[0][1] = '1';
	screen.map_data[0][2] = '1';
	screen.map_data[0][3] = '1';
	screen.map_data[0][4] = '1';
	screen.map_data[1][0] = '1';
	screen.map_data[1][1] = '0';
	screen.map_data[1][2] = '0';
	screen.map_data[1][3] = '0';
	screen.map_data[1][4] = '1';
	screen.map_data[2][0] = '1';
	screen.map_data[2][1] = '0';
	screen.map_data[2][2] = '0';
	screen.map_data[2][3] = '0';
	screen.map_data[2][4] = '1';
	screen.map_data[3][0] = '1';
	screen.map_data[3][1] = '0';
	screen.map_data[3][2] = '0';
	screen.map_data[3][3] = 'N';
	screen.map_data[3][4] = '1';
	screen.map_data[4][0] = '1';
	screen.map_data[4][1] = '1';
	screen.map_data[4][2] = '1';
	screen.map_data[4][3] = '1';
	screen.map_data[4][4] = '1';
	return (screen);
}

int		main(void)
{
	t_screen	screen;
	t_player	player;

	player.x = 3;
	player.y = 3;
	player.facing = 0.0;
    
	screen = set_map(screen);	
	screen.player = &player;
	screen.mlx = mlx_init();
	if (!screen.mlx)
		return (0);
	screen.win = mlx_new_window(screen.mlx, WIDTH, HEIGHT, "Cub3D, Son");
	if (!screen.win)
		return (0);

	mlx_expose_hook(screen.win, &ft_draw, &screen);
	mlx_hook(screen.win, 2, 1L<<0, &ft_parse_keys, &screen);
	mlx_loop_hook(screen.mlx, &ft_draw, &screen);

	// Wait so user can see window
	mlx_loop(screen.mlx);
	
//	mlx_destroy_window(screen.mlx, screen.win);
	ft_close_screen(screen.win, &screen);
	exit(EXIT_SUCCESS);
}
