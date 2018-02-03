CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

all : Board/Board.o Piece/Piece.o

Board/Board.o : Board/Board.c
	$(CC) $(CFLAGS) -c -o Board/Board.o Board/Board.c

Piece/Piece.o : Piece/Piece.c
	$(CC) $(CFLAGS) -c -o Piece/Piece.o Piece/Piece.c

clean :
	rm Board/Board.o Piece/Piece.o
