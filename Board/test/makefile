CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes

main : main.o ../../errors/def_hndl.o ../../Piece/Piece.o ../Board.o ../../display/display_cli.o
	$(CC) main.o ../../errors/def_hndl.o ../../Piece/Piece.o ../Board.o ../../display/display_cli.o -o main

main.o :  main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../../display/display_cli.o : ../../display/display_cli.c
	$(CC) $(CFLAGS) -c -o ../../display/display_cli.o ../../display/display_cli.c

../Board.o : ../Board.c
	$(CC) $(CFLAGS) -c -o ../Board.o ../Board.c

../../Piece/Piece.o : ../../Piece/Piece.c
	$(CC) $(CFLAGS) -c -o ../../Piece/Piece.o ../../Piece/Piece.c

../../errors/def_hndl.o : ../../errors/def_hndl.c
	$(CC) $(CFLAGS) -c -o ../../errors/def_hndl.o ../../errors/def_hndl.c

clean :
	rm main main.o ../../display/display_cli.o ../../errors/def_hndl.o ../../Piece/Piece.o ../Board.o
