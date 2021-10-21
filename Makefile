NAME = cub3D

SRC_DIR = ./srcs/
LIBFT_DIR = ./libft/
LIBFT = libft.a
MLX_DIR = ./minilibx-linux/
MLX = libmlx.a

CC = gcc
FLAGS = -Wall -Wextra -Werror

SRCS = $(SRC_DIR)draw.c \
	   $(SRC_DIR)errors.c \
	   $(SRC_DIR)images.c \
	   $(SRC_DIR)keys.c \
	   $(SRC_DIR)main.c \
	   $(SRC_DIR)parser.c \
	   $(SRC_DIR)parser_config_other.c \
	   $(SRC_DIR)parser_config_textures.c \
	   $(SRC_DIR)parser_data.c \
	   $(SRC_DIR)parser_map.c \
	   $(SRC_DIR)parser_to_screen.c \
	   $(SRC_DIR)parser_utils.c \
	   $(SRC_DIR)screen.c \
	   $(SRC_DIR)screenshot.c \
	   $(SRC_DIR)raycast.c \
	   $(SRC_DIR)raycast_sprites.c \
	   $(SRC_DIR)raycast_sprites_utils.c

OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_DIR)$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(MLX_DIR)$(MLX) $(LIBFT_DIR)$(LIBFT) $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	bash ./tests/test_invalid_maps.sh

leaks:
	bash ./tests/test_invalid_maps_leaks.sh

.PHONY: all clean fclean re test leaks
