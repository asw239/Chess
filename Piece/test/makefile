CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main: main.o ../Piece.o
	$(CC) main.o ../Piece.o -o main

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../Piece.o : ../Piece.c
	$(CC) $(CFLAGS) -c -o ../Piece.o ../Piece.c

clean :
	rm main main.o ../Piece.o
