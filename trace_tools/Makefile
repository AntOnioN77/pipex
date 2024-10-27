NAME = libtrace_tools.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SOURCES = get_id.c log_message.c
OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)
$(NAME): $(OBJECTS) trace_tools.h
	ar rcs $(NAME) $(OBJECTS)