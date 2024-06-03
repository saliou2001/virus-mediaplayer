CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

CalculMoyenne.: CalculMoyenne.c
	$(CC) -o CalculMoyenne CalculMoyenne.c $(CFLAGS) $(LIBS)
