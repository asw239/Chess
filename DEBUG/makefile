CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../errors/def_hndl.o ../errors/error_utilities.o ../Piece/Piece.o ../Board/Board.o ../game_logic/game_logic.o ../display/display_cli.o
	$(CC) main.o ../errors/def_hndl.o ../errors/error_utilities.o ../Piece/Piece.o ../Board/Board.o ../game_logic/game_logic.o ../display/display_cli.o -o main

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../errors/def_hndl.o : ../errors/def_hndl.c
	$(CC) $(CFLAGS) -c -o ../errors/def_hndl.o ../errors/def_hndl.c

../errors/error_utilities.o : ../errors/error_utilities.c
	$(CC) $(CFLAGS) -c -o ../errors/error_utilities.o ../errors/error_utilities.c

../Piece/Piece.o : ../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../Piece/Piece.o ../Piece/Piece.c

../Board/Board.o : ../Board/Board.c
	$(CC) $(CFLAGS) -c -o ../Board/Board.o ../Board/Board.c

../game_logic/game_logic.o : ../game_logic/game_logic.c
	$(CC) $(CFLAGS) -c -o ../game_logic/game_logic.o ../game_logic/game_logic.c

../display/display_cli.o : ../display/display_cli.c
	$(CC) $(CFLAGS) -c -o ../display/display_cli.o ../display/display_cli.c

clean :
	rm main main.o ../errors/def_hndl.o ../errors/error_utilities.o ../Piece/Piece.o ../Board/Board.o ../game_logic/game_logic.o ../display/display_cli.o
