CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

CalculatriceBasique: CalculatriceBasique.c
	$(CC) -o CalculatriceBasique CalculatriceBasique.c $(CFLAGS) $(LIBS)
