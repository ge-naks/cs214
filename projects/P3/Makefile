CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address

SRC = mysh.c

TARGET = mysh

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
