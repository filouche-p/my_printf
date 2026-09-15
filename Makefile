NAME = my_printf
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
SRCS = my_printf.c
OBJS = $(SRCS:.c=.o)
HEADERS = write_formated.h write.h utils.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re