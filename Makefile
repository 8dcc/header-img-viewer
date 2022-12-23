
CC=gcc
CFLAGS=-Wall -Wextra
BIN=header-img-viewer.out

.PHONY: all clean

all: $(BIN)

$(BIN): src/main.c src/test/img.h
	$(CC) $(CFLAGS) src/main.c -o $@ `sdl2-config --cflags --libs`

clean:
	rm -f $(BIN)

