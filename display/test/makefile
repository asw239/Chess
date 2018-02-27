CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../../errors/def_hndl.o ../../Piece/Piece.o ../../Board/Board.o ../../game_logic/game_logic.o ../display_cli.o
	$(CC) main.o ../../errors/def_hndl.o ../../Piece/Piece.o ../../Board/Board.o ../../game_logic/game_logic.o ../display_cli.o -o main

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../../Board.o : ../../Board/Board.c
	$(CC) $(CFLAGS) -c -o ../../Board/Board.o ../../Board/Board.c

../display_cli.o : ../display_cli.c
	$(CC) $(CFLAGS) -c -o ../display_cli.o ../display_cli.c

../../game_logic/game_logic.o : ../../game_logic/game_logic.c
	$(CC) $(CFLAGS) -c -o ../../game_logic/game_logic.o ../../game_logic/game_logic.c

../../Piece/Piece.o : ../../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../../Piece/Piece.o ../../Piece/Piece.c

../../errors/def_hndl.o : ../../errors/def_hndl.c
	$(CC) $(CFLAGS) -c -o ../../errors/def_hndl.o ../../errors/def_hndl.c

clean :
	rm main.o ../../Board/Board.o ../display_cli.o ../../game_logic/game_logic.o ../../Piece/Piece.o ../../errors/def_hndl.o
