#include "Board.h"
#include "../Piece/Piece.h"
#include <stdlib.h>
#include <stdint.h>

#define BOARD_SIZE 8

struct BoardStruct{
	Piece **board_arr;
};

Board board_create(void)
{
	Board b;
	b = malloc(sizeof(*b));
	if(!b)
		; // callback for memory allocation fail

	b->board_arr = malloc(sizeof(Piece*) * 8);
	if(!b->board_arr)
		; // callback for memory allocation fail

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		b->board_arr[i] = calloc(8, sizeof(Piece));
		if(!b->board_arr[i])
			; // callback for memory allocation fail
	}

	return b;
}

void board_destroy(Board b)
{
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		free(b->board_arr[i]);

	free(b->board_arr);
	free(b);
}
