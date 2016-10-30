
CC=gcc
CFLAGS=-g -Wall

RM = rm -r

all: main

main: main.c environment.c execute.c path.c strings.c
	$(CC) $(CFLAGS) -o main main.c environment.c execute.c path.c strings.c

clean:
	$(RM) main
