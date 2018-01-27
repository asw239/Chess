CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../../Piece/Piece.o ../Board.o
	$(CC) main.o ../Board.o -o main

main.o :  main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../../Piece/Piece.o : ../../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../../Piece/Piece.o ../../Piece/Piece.c

../Board.o : ../Board.c
	$(CC) $(CFLAGS) -c -o ../Board.o  ../Board.c

clean :
	rm main main.o ../../Piece/Piece.o ../Board.o
