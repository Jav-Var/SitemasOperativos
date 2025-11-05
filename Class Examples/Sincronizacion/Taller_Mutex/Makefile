CC := gcc
CFLAGS := -Wall -Wextra -O2 -g

SRC := main.c
TARGET := main

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
