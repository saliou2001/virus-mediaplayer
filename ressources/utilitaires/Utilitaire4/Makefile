CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

CalculPourcentage: CalculPourcentage.c
	$(CC) -o CalculPourcentage CalculPourcentage.c $(CFLAGS) $(LIBS)
