cc = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../../Piece/Piece.o ../../Board/Board.o ../display_cli.o
	$(CC) main.o ../../Piece/Piece.o ../../Board/Board.o ../display_cli.o -o main

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../display_cli.o : ../display_cli.c
	$(CC) $(CFLAGS) -c -o ../display_cli.o ../display_cli.c

../../Board/Board.o : ../../Board/Board.c
	$(CC) $(CFLAGS) -c -o ../../Board/Board.o ../../Board/Board.c

../../Piece/Piece.o : ../../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../../Piece/Piece.o ../../Piece/Piece.c

clean :
	rm main main.o ../../Piece/Piece.o ../../Board/Board.o ../display_cli.o
