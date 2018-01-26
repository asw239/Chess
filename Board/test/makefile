CC = gcc
CFLAGS = -Wall -Wextra -Wfloat-equal -Wpointer-arith -Wcast-align

main : main.o ../Board.o
	$(CC) main.o ../Board.o -o main

main.o :  main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

../Board.o : ../Board.o
	$(CC) $(CFLAGS) -c -o ../Board.o  ../Board.c

clean :
	rm main main.o ../Board.o
