NAME = libtrace_tools.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SOURCES = get_id.c log_message.c log_end.c test_sample_fd.c monitor_pipe.c
OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)
$(NAME): $(OBJECTS) trace_tools.h
	ar rcs $(NAME) $(OBJECTS)
clean:
	rm $(OBJECTS)
fclean: clean
	rm $(NAME)
re: fclean all