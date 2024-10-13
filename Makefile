NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -I. -I/libft/headers
DBGFLAGS = -g3
LDFLAGS = -L ./libft -lft
SOURCES = pipex.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = libft/headers/libft.h libft/headers/ring.h circular_array.h

all: $(NAME)
debug: CFLAGS += $(DBGFLAGS)
debug: fclean all
$(NAME): libft/libft.a $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OBJECTS) $(LDFLAGS) -o $(NAME)
libft/libft.a:
	make extra -C libft
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(LDFLAGS)
.PHONY: clean fclean all
clean:
	rm -f $(OBJECTS)
	make clean -C libft
fclean: clean
	rm -f $(NAME)
	rm -f libft/libft.a