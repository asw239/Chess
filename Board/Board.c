#include "Board.h"
#include <stdlib.h>

static const uint_fast8_t BOARD_SIZE = 8;

struct BoardStruct{
	Piece **board_arr;
	uint_fast8_t *turn;	// 1->white, 0->black
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

	b->turn = malloc(sizeof(uint_fast8_t));
	if(!b->turn)
			; // callback for memory allocation fail

	return b;
}

void board_destroy(Board b)
{
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++)
			if(b->board_arr[i][j])
				piece_destroy(b->board_arr[i][j]);

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		free(b->board_arr[i]);

	free(b->board_arr);
	free(b->turn);
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

void board_set_turn(Board b, uint_fast8_t t)
{
	*(b->turn) = t;
}

void board_flip_turn(Board b)
{
	*(b->turn) ? (*(b->turn) = 0) : (*(b->turn) = 1);
}

uint_fast8_t board_get_turn(Board b)
{
	return *(b->turn);
}

Piece board_remove_piece(Board b, Piece p, bool destroy)
{
	b->board_arr[piece_get_x(p)][piece_get_y(p)] = NULL;

	if(destroy){
		piece_destroy(p);
		return NULL;
	}else{
		return p;
	}
}
