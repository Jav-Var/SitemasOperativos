CC = gcc
CFLAGS = -Wall -pthread -lm

mutex_example: mutex_example.o
	$(CC) -o $@ $^

mutex_example.o: mutex_example.c
	$(CC) $(CFLAGS) -c -o $@ $< 