NAME = cub3D

SRC_DIR = ./srcs/
LIBFT_DIR = ./libft/
LIBFT = libft.a
MLX_DIR = ./minilibx-linux/
MLX = libmlx.a

SRCS = main.c parser.c screenshot.c screen.c raycast.c draw.c images.c \
		keys.c errors.c

OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(NAME):
	@$(MAKE) bonus -C $(LIBFT_DIR)
	@$(MAKE) -C $(MLX_DIR)
	@$(MAKE) -C $(SRC_DIR)

clean:
	@$(MAKE) clean -C $(LIBFT_DIR)
	@$(MAKE) clean -C $(MLX_DIR)
	@$(MAKE) clean -C $(SRC_DIR)

fclean: 
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@$(MAKE) clean -C $(MLX_DIR)
	@$(MAKE) fclean -C $(SRC_DIR)

re: fclean all

.PHONY: all clean fclean re $(NAME)
