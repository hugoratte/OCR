CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image) -lSDL2 -lSDL2_image -lm `sdl2-config --cflags --libs`
EXEC= main

SRC= Image_Splitting.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${DEP} ${EXEC}
