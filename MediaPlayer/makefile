# Compiler
CC := gcc

# Compiler flags
CFLAGS := `pkg-config --cflags gtk+-3.0 gstreamer-1.0`
LDFLAGS := `pkg-config --libs gtk+-3.0 gstreamer-1.0`

# Source files
SRCS := mediaplayer.c

# Object files
OBJS := $(SRCS:.c=.o)

# Executable store it to a anotther file
TARGET := mediaplayer.exe


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
move:
	mv $(TARGET) testVirus/

clean:
	rm -f $(OBJS) $(TARGET)

# command gcc -o MediaPlayer MediaPlayer.c `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0`

