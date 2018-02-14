CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

all : Board/Board.o Piece/Piece.o game_logic/game_logic.o display/display_cli.o

Board/Board.o : Board/Board.c
	$(CC) $(CFLAGS) -c -o Board/Board.o Board/Board.c

Piece/Piece.o : Piece/Piece.c
	$(CC) $(CFLAGS) -c -o Piece/Piece.o Piece/Piece.c

game_logic/game_logic.o : game_logic/game_logic.c
	$(CC) $(CFLAGS) -c -o game_logic/game_logic.o game_logic/game_logic.c

display/display_cli.o : display/display_cli.c
	$(CC) $(CFLAGS) -c -o display/display_cli.o display/display_cli.c

clean :
	rm Board/Board.o Piece/Piece.o game_logic/game_logic.o display/display_cli.o
