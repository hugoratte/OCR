CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -Werror -std=c99 $$(pkg-config --cflags sdl gtk+-3.0)
LDFLAGS=
LDLIBS=-lm $$(pkg-config --libs sdl SDL_image SDL_gfx gtk+-3.0) -lSDL2 -lSDL2_image -lm `sdl2-config --cflags --libs`
EXEC= main

SRC= main.c image_treatment/image_treatment.c Image_SPLITTING/Image_Splitting.c neural_network/neural_network.c neural_network/neural_network_tools.c neural_network/sdl_tools.c neural_network/main_nn.c grid_solver/main_solver.c grid_solver/solver.c
OBJ= ${SRC:.c=.o}
DEP= ${SRC:.c=.d}

all: ${EXEC}



${EXEC}: ${OBJ}

	${CC} ${LDLIBS} ${LDFLAGS} -o $@ $^ 



clean:
	rm -f ${OBJ} ${DEP} ${EXEC} main
