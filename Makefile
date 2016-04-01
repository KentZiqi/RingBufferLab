CC = gcc
CFLAGS = -g -Wall -Werror -std=c99

all: ringbuf.c
	$(CC) $(CFLAGS) -o ringbuf ringbuf.c -lpthread
