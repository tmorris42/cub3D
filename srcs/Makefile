NAME = ../cub3D

LIBFT_DIR = ../libft/
LIBFT = libft.a
MLX_DIR = ../minilibx-linux/
MLX = libmlx.a

SRCS = draw.c \
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

OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

$(MLX_DIR)$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJS) $(LIBFT_DIR)$(LIBFT) $(MLX_DIR)$(MLX)
	gcc -Wall -Wextra -Werror $(OBJS) -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	gcc -Wall -Wextra -Werror -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug:
	gcc -c -g3 $(SRCS) -fsanitize=address
	gcc -g3 $(OBJS) -L./libft/ -lft -L./minilibx-linux/ -lmlx -lXext -lX11 -lbsd -lm -o debug.o -fsanitize=address
	rm -f $(OBJS)

.PHONY: all clean fclean re debug
