CC = gcc
CFLAGS = -pipe -g
TARGET = build/main
SRC = main.c matrix.c
WARN = -Wall -Wextra -Wpedantic

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

warn: CFLAGS += $(WARN)
warn: $(TARGET)

clean:
	rm -f $(TARGET)

