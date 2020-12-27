NAME = cub3D

LIBFT_DIR = ./libft/
LIBFT = libft.a
MLX_DIR = ./minilibx-linux/
MLX = libmlx.a

SRCS = main.c parser.c screenshot.c

OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(LIBFT_DIR)$(LIBFT):
	$(MAKE) bonus -C $(LIBFT_DIR)

$(MLX_DIR)$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJS) $(LIBFT_DIR)$(LIBFT) $(MLX_DIR)$(MLX)
	gcc $(OBJS) -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	gcc -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug:
	gcc -c -g3 $(SRCS) -fsanitize=address
	gcc -g3 $(OBJS) -L./libft/ -lft -L./minilibx-linux/ -lmlx -lXext -lX11 -lbsd -lm -o debug.o -fsanitize=address
	rm -f $(OBJS)

.PHONY: all clean fclean re debug $(LIBFT_DIR)$(LIBFT) $(MLX_DIR)$(MLX)

