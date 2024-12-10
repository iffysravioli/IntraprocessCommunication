
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -lpthread

all: serv client

serv: serv.c list.c list.h
	$(CC) $(CFLAGS) serv.c list.c -o serv

client: cli.c
	$(CC) $(CFLAGS) cli.c -o client

clean:
	rm -f serv client
