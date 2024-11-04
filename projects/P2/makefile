CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = words.c test_file.c

TARGET = words

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
