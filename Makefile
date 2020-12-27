NAME = cub3d

SRCS = main.c parser.c screenshot.c

OBJS = ${SRCS:.c=.o}

$(NAME): $(OBJS)
	gcc $(OBJS) -L./libft/ -lft -L./minilibx-linux/ -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	gcc -c $< -o $@

all: $(NAME)

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
