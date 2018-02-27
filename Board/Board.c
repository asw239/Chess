#include "Board.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>
#include <stdio.h>

#define FILE_NAME "Board.c"

static const uint_fast8_t BOARD_SIZE = 8;
static void (*err_fnc_ptr)(enum ErrorCode err, const char *msg) = def_hndl;

struct BoardStruct{
	Piece **board_arr;
	enum PieceColor *turn;
};

Board board_create(void)
{
	#define FUNCT_NAME "Board board_create(void)"

	Board b;
	b = malloc(sizeof(*b));
	if(!b)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNCT_NAME);

	b->board_arr = malloc(sizeof(Piece*) * BOARD_SIZE);
	if(!b->board_arr)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNCT_NAME);

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		b->board_arr[i] = calloc(BOARD_SIZE, sizeof(Piece));
		if(!b->board_arr[i])
			err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", "
				FUNCT_NAME);
	}

	b->turn = malloc(sizeof(uint_fast8_t));
	if(!b->turn)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNCT_NAME);

	#undef FUNC_NAME

	return b;
}

void board_destroy(Board *b)
{
	#define FUNC_NAME "void board_destroy(Board *b)"

	if(!b)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++)
			if((*b)->board_arr[i][j])
				piece_destroy(&(*b)->board_arr[i][j]);

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		free((*b)->board_arr[i]);

	free((*b)->board_arr);
	free((*b)->turn);
	free(*b);

	*b = NULL;
}

Piece **board_get_arr(Board b)
{
	#define FUNC_NAME "Piece **board_get_arr(Board b)"

	if(!b)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return b->board_arr;
}

uint_fast8_t board_get_size(void)
{

	return BOARD_SIZE;
}

void board_set_turn(Board b, enum PieceColor c)
{
	#define FUNC_NAME "void board_set_turn(Board b, uint_fast8_t)"

	if(!b)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	if(c != BLACK && c != WHITE)
		err_fnc_ptr(INVALID_ENUM_PARAM,
			"In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	*(b->turn) = c;
}

void board_flip_turn(Board b)
{
	#define FUNC_NAME "void board_flip_turn(Board b)"

	if(!b)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	*(b->turn) == WHITE ? (*(b->turn) = BLACK) : (*(b->turn) = WHITE);
}

enum PieceColor board_get_turn(Board b)
{
	#define FUNC_NAME "uint_fast8_t board_get_turn(Board b)"

	if(!b)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return *(b->turn);
}

Piece board_remove_piece(Board b, Piece *p, bool destroy)
{
	#define FUNC_NAME "Piece board_remove_piece(Board b, Piece *p, "\
		"bool destroy)"

	if(!b || !p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	bool found = false;
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++)
			if(b->board_arr[i][j] == *p)
				found = true;

	if(!found)
		err_fnc_ptr(PIECE_NOT_IN_BOARD,
			"In file " FILE_NAME ", " FUNC_NAME);

	b->board_arr[piece_get_x(*p)][piece_get_y(*p)] = NULL;

	if(destroy){
		piece_destroy(p);
		return NULL;
	}else{
		return *p;
	}

	#undef FUNC_NAME
}
