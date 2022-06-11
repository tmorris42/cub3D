NAME = cub3D

SRC_DIR = ./srcs/
LIBFT_DIR = ./libft/
LIBFT = libft.a
MLX_DIR = ./minilibx-linux/
MLX = libmlx.a

FILES = draw.c \
		errors.c \
		images.c \
		keys.c \
		main.c \
		parser.c \
		parser_config_other.c \
		parser_config_textures.c \
		parser_data.c \
		parser_map.c \
		parser_to_screen.c \
		parser_utils.c \
		screen.c \
	   	screenshot.c \
	   	raycast.c \
	 	raycast_sprites.c \
	   	raycast_sprites_utils.c
CC = gcc
FLAGS = -Wall -Wextra -Werror

BONUS_FILES = draw_bonus.c \
		draw_utils_bonus.c \
		errors.c \
		images.c \
		keys_bonus.c \
		main_bonus.c \
		next_level_bonus.c \
		object_collision_bonus.c \
		parser_bonus.c \
		parser_config_bonus.c \
		parser_config_other.c \
		parser_config_textures.c \
		parser_data_bonus.c \
		parser_map.c \
		parser_to_screen_bonus.c \
		parser_utils.c \
		screen_bonus.c \
	   	screenshot.c \
	   	raycast.c \
	 	raycast_sprites.c \
	   	raycast_sprites_utils.c

SRCS = $(addprefix $(SRC_DIR),$(FILES))
OBJS = ${SRCS:.c=.o}

BONUS_NAME = bonus3D
BONUS_DIR = ./bonus/
BONUS_SRCS = $(addprefix $(BONUS_DIR),$(BONUS_FILES))
BONUS_OBJS = ${BONUS_SRCS:.c=.o}

all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_DIR)$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(MLX_DIR)$(MLX) $(LIBFT_DIR)$(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

$(BONUS_OBJS): %.o : %.c
	gcc -Wall -Wextra -Werror -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

$(BONUS_NAME): $(MLX_DIR)$(MLX) $(LIBFT_DIR)$(LIBFT) $(BONUS_OBJS)
	gcc -Wall -Wextra -Werror $(BONUS_OBJS) -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd -lm -o $(BONUS_NAME)

bonus: $(BONUS_NAME)

test:
	bash ./tests/test_invalid_maps.sh

leaks:
	bash ./tests/test_invalid_maps_leaks.sh

.PHONY: all clean fclean re test leaks bonus
