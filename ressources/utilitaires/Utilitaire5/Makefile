CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0)
LIBS=$(shell pkg-config --libs gtk+-3.0)

ConvertisseurUnite: ConvertisseurUnite.c
	$(CC) -o CConvertisseurUnite ConvertisseurUnite.c $(CFLAGS) $(LIBS)
