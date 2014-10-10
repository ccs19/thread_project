CC = gcc
CFLAGS = -g -Wall
OBJECTS = *.c
NAME = lizard
TODELETE = $(NAME) *.o
LIBS = -lpthread

mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TODELETE)
