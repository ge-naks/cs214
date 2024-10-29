CC = gcc
CFLAGS = -Wall -g

all: test_file

test_file: words.o test_file.o
	$(CC) $(CFLAGS) -o test_file words.o test_file.o

words.o: words.c words.h
	$(CC) $(CFLAGS) -c words.c

test_file.o: test_file.c words.h
	$(CC) $(CFLAGS) -c test_file.c

clean:
	rm -f *.o test_file
