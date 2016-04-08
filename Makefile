CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu99

all: ringbuf.c
	$(CC) $(CFLAGS) -o ringbuf ringbuf.c -lpthread
