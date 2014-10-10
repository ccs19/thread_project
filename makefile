CC = gcc
CFLAGS = -g -Wall
OBJECTS = *.c
NAME = lizard
TODELETE = $(NAME) *.o
LIBS = -lpthread

$(NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

.PHONY: clean
clean:
	rm $(TODELETE)
