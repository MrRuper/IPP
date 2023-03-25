CC 	 = gcc
CPPFLAGS =
CFLAGS   = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2
LDFLAGS  =

.PHONY: all clean

all: game

game: main.o game.o
main.o: main.c #zmienic na game_example!
game.o: game.h game.c	

clean:
	rm -f *.o game


