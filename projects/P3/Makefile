CC = gcc
CFLAGS =  -Wall -fsanitize=address

SRC = mysh.c arraylist.c

TARGET = mysh

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
