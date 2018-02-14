CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../../Board/Board.o ../../display/display_cli.o ../game_logic.o ../../Piece/Piece.o
	$(CC) -o main main.o ../../Board/Board.o  ../../display/display_cli.o ../game_logic.o ../../Piece/Piece.o

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../../Board.o : ../../Board/Board.c
	$(CC) $(CFLAGS) -c -o ../../Board/Board.o ../../Board/Board.c

../../display/display_cli.o : ../../display/display_cli.c
	$(CC) $(CFLAGS) -c -o ../../display/display_cli.o ../../display/display_cli.c

../game_logic.o : ../game_logic.c
	$(CC) $(CFLAGS) -c -o ../game_logic.o ../game_logic.c

../../Piece/Piece.o : ../../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../../Piece/Piece.o ../../Piece/Piece.c

clean :
	rm main.o ../../Board/Board.o ../../display/display_cli.o ../game_logic.o ../../Piece/Piece.o
