CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl gtk+-3.0)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image gtk+-3.0) -lSDL2 -lSDL2_image -lm `sdl2-config --cflags --libs`
EXEC= main

SRC= GUI.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${DEP} ${EXEC} main
