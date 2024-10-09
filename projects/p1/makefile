CC = gcc
CFLAGS = -Wall -g

all: memgrind

memgrind: mymalloc.o memgrind.o
	$(CC) $(CFLAGS) -o memgrind mymalloc.o memgrind.o

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CFLAGS) -c memgrind.c

clean:
	rm -f *.o memgrind
