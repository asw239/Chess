CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

Board/Board.o : Board/Board.c
	$(CC) $(CFLAGS) -c -o Board/Board.o Board/Board.c

clean :
	rm Board/Board.o
