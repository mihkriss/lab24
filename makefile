CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g
SRC = tree.c lab24.c
PROG = lab24

build:
	$(CC) $(CFLAGS) $(SRC) -o $(PROG)

clean:
	rm $(PROG)

