CC = gcc
CFLAGS = -I. -g

backendmake: ../frame.c ../frame.h ../frame_ops.c ../frame_ops.h ../game_node.c ../game_node.h vs_computer_v0.c
	${CC} -o test ../frame.c ../frame_ops.c ../game_node.c vs_computer_v0.c ${CFLAGS}
