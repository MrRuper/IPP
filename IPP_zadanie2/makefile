CC 	 = gcc
CPPFLAGS =
CFLAGS   = -Wall -Wextra -Wno-implicit-fallthrough -std=c17 -O2
LDFLAGS  = -lncurses

.PHONY: all clean

all: game

game: game_main.o game.o
game_main.o: game_main.c game.h
game.o: game.h game.c

valgrind_test:
	valgrind --error-exitcode=123 -q --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./game $(ARGS)
clean:
	rm -f *.o game

