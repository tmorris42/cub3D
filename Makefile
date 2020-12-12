NAME = cub3d

SRCS = main.c

OBJS = ${SRCS:.c=.o}

$(NAME): $(OBJS)
	gcc $(OBJS) -L./minilibx/ -lmlx -lXext -lX11 -lbsd -lm -o $(NAME)

$(OBJS): %.o : %.c
	gcc -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
