CC = gcc
CFLAGS = -Wall -pthread -lm

.PHONY: all

all: server client

server: server.o
	$(CC) -o $@ $^

server.o: server.c
	$(CC) $(CFLAGS) -c -o $@ $< 

client: client.o
	$(CC) -o $@ $^

client.o: client.c
	$(CC) $(CFLAGS) -c -o $@ $<