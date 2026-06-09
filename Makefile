CC = gcc
CFLAGS = -std=c99 -Wall -O2
TARGET = editor

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c -lm

clean:
	rm -f $(TARGET)
