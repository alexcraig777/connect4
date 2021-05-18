CC = gcc
CFLAGS = -Wall -g

backendmake: ../nine_byte.c ../nine_byte.h \
	../small_frame.c ../frame.h \
	../frame_ops.c ../frame_ops.h \
	../game_node.c ../game_node.h \
	vs_computer_v1.c
	${CC} -o test ../nine_byte.c ../small_frame.c ../frame_ops.c ../game_node.c vs_computer_v1.c ${CFLAGS}
