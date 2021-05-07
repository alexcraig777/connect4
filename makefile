CC = gcc
CFLAGS = -I. -g

backendmake: frame.c heap.c
        ${CC} -o test frame.c heap.c ${CFLAGS}
