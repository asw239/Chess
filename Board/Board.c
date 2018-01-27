#include "Board.h"
#include <stdlib.h>

static const uint_fast8_t BOARD_SIZE = 8;

struct BoardStruct{
	Piece **board_arr;
};

Board board_create(void)
{
	Board b;
	b = malloc(sizeof(*b));
	if(!b)
		; // callback for memory allocation fail

	b->board_arr = malloc(sizeof(Piece*) * BOARD_SIZE);
	if(!b->board_arr)
		; // callback for memory allocation fail

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		b->board_arr[i] = calloc(BOARD_SIZE, sizeof(Piece));
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

Piece **board_get_arr(Board b)
{
	return b->board_arr;
}

uint_fast8_t board_get_size(void)
{
	return BOARD_SIZE;
}
