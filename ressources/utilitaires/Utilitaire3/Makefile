CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

CalculPgcd: CalculPgcd.c
	$(CC) -o CalculPgcd CalculPgcd.c $(CFLAGS) $(LIBS)
