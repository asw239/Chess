CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align

Board/Board.o : Board/Board.c
	$(CC) $(CFLAGS) -c -o Board/Board.o Board/Board.c

clean :
	rm Board/Board.o
