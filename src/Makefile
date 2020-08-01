CC = cc
LIBS = -lSDL2 -lm
CFLAGS = -Wall -Wextra -O3

SRC = main.c \
	graphics.c \
	model.c \
	color.c \
	utils.c \
	math/maths.c \
	math/vector.c \
	math/matrix.c \
	

#OBJ = ${SRC:.c=.o}

#.c.o:
#	${CC} -c ${CFLAGS} $<

main: ${SRC}
	${CC} ${CFLAGS} -o $@ ${SRC} ${LIBS}

clean:
	rm ${OBJ} main
