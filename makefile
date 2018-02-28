CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

all : errors/def_hndl.o Piece/Piece.o Board/Board.o game_logic/game_logic.o display/display_cli.o

errors/def_hndl.o : errors/def_hndl.c
	$(CC) $(CFLAGS) -c -o errors/def_hndl.o errors/def_hndl.c

Piece/Piece.o : Piece/Piece.c
	$(CC) $(CFLAGS) -c -o Piece/Piece.o Piece/Piece.c

Board/Board.o : Board/Board.c
	$(CC) $(CFLAGS) -c -o Board/Board.o Board/Board.c

game_logic/game_logic.o : game_logic/game_logic.c
	$(CC) $(CFLAGS) -c -o game_logic/game_logic.o game_logic/game_logic.c

display/display_cli.o : display/display_cli.c
	$(CC) $(CFLAGS) -c -o display/display_cli.o display/display_cli.c

clean :
	rm errors/def_hndl.o Piece/Piece.o Board/Board.o game_logic/game_logic.o display/display_cli.o
