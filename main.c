#include <stdio.h> //remove this
#include <stdlib.h> //for exit() EXIT_SUCCESS, EXIT_FAILURE
#include "header.h"
//#include "libft/libft.h"

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

typedef struct		s_screen
{
	void	*mlx;
	void	*win;
}					t_screen;

int		ft_get_color(int r, int g, int b)   // use bit-shift instead   ex:  t  << 16
{
	return ((255 * ((255 * r) + g)) + b);
}

int		ft_draw(t_screen *screen)
{
	mlx_pixel_put(screen->mlx, screen->win, 10, 10, 255);
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
	else
		printf(" : %d\n", key);
	return (1);
}

int		main(void)
{
	t_screen	screen;

	screen.mlx = mlx_init();
	if (!screen.mlx)
		return (0);
	screen.win = mlx_new_window(screen.mlx, WIDTH, HEIGHT, "Cub3D, Son");
	if (!screen.win)
		return (0);

	mlx_expose_hook(screen.win, &ft_draw, &screen);
	mlx_hook(screen.win, 2, 1L<<0, &ft_parse_keys, &screen);

	// Wait so user can see window
	mlx_loop(screen.mlx);
	
//	mlx_destroy_window(screen.mlx, screen.win);
	ft_close_screen(screen.win, &screen);
	exit(EXIT_SUCCESS);
}
