CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main: main.o ../../errors/def_hndl.o ../Piece.o
	$(CC) main.o ../../errors/def_hndl.o ../Piece.o -o main

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../Piece.o : ../Piece.c
	$(CC) $(CFLAGS) -c -o ../Piece.o ../Piece.c

../../errors/def_hndl.o : ../../errors/def_hndl.c
	$(CC) $(CFLAGS) -c -o ../../errors/def_hndl.o ../../errors/def_hndl.c

clean :
	rm main main.o ../../errors/def_hndl.o ../Piece.o
