#include "Board.h"
#include "../errors/def_hndl.h"
#include "../game_logic/game_logic.h"
#include <stdlib.h>
#include <stdio.h>

#define FILE_NAME "Board.c"

static const uint_fast8_t BOARD_SIZE = 8;

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

struct BoardStruct{
	Piece **board_arr;
	enum PieceColor *turn;
	Piece *w_capture_list;
	Piece *b_capture_list;
	bool *unmoved_pieces;
};

Board board_create(void)
{
	#define FUNC_NAME "Board board_create(void)"

	Board b;
	b = malloc(sizeof(*b));
	if(!b){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file" FILE_NAME
			", " FUNC_NAME);
	}

	b->board_arr = malloc(sizeof(Piece*) * BOARD_SIZE);
	if(!b->board_arr){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file" FILE_NAME
			", " FUNC_NAME);
	}
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		b->board_arr[i] = calloc(BOARD_SIZE, sizeof(Piece));
		if(!b->board_arr[i]){
			if(!err_fnc_arr[MEM_FAIL])
				err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
					FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file "
					FILE_NAME ", " FUNC_NAME);
		}
	}

	b->turn = malloc(sizeof(uint_fast8_t));
	if(!b->turn){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	b->unmoved_pieces = malloc(sizeof(bool) * UNMOVED_PIECES_COUNT);
	if(!b->unmoved_pieces){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME

	b->b_capture_list = NULL;
	b->w_capture_list = NULL;

	return b;
}

void board_destroy(Board *b)
{
	#define FUNC_NAME "void board_destroy(Board *b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME
			", " FUNC_NAME);
	}

	#undef FUNC_NAME

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++)
			if((*b)->board_arr[i][j])
				piece_destroy(&(*b)->board_arr[i][j]);

	if((*b)->w_capture_list){
		for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
			if((*b)->w_capture_list[i])
				piece_destroy(&((*b)->w_capture_list[i])); 
		free((*b)->w_capture_list);
	}
	if((*b)->b_capture_list){
		for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
			if((*b)->b_capture_list[i])
				piece_destroy(&((*b)->b_capture_list[i])); 
		free((*b)->b_capture_list);
	}

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		free((*b)->board_arr[i]);
	free((*b)->board_arr);

	free((*b)->turn);
	free((*b)->unmoved_pieces);
	free(*b);

	*b = NULL;
}

void board_link_piece(Board b, Piece p, uint_fast8_t r, uint_fast8_t c)
{
	#define FUNC_NAME "void board_link_piece(Board b, Piece p, "\
		"uint_fast8_t r, uint_fast8_t c"

	if(!b || !p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME
			", " FUNC_NAME);

	}else if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file" FILE_NAME ", " FUNC_NAME);

	}else if(b->board_arr[r][c]){
		if(!err_fnc_arr[BOARD_NONEMPTY_SQUARE])
			err_fnc_arr[GLOBAL_ERROR](BOARD_NONEMPTY_SQUARE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[BOARD_NONEMPTY_SQUARE](
				BOARD_NONEMPTY_SQUARE, "In file " FILE_NAME ", "
				FUNC_NAME);
	}

	#undef FUNC_NAME

	b->board_arr[r][c] = p;
	piece_set_pos(p, r, c);
}

void board_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	#define FUNC_NAME "void board_move_piece(Board b, uint_fast8_t r_old, "\
		"uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file " FILE_NAME
				", " FUNC_NAME);

	}else if(r_old >= BOARD_SIZE || c_old >= BOARD_SIZE ||
		r_new >= BOARD_SIZE || c_new >= BOARD_SIZE){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);

	}else if(r_old == r_new && c_old == c_new){
		if(!err_fnc_arr[PIECE_MOVE_SAME_POS])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_SAME_POS](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);

	}else if(!b->board_arr[r_old][c_old]){
		if(!err_fnc_arr[BOARD_EMPTY_SQUARE])
			err_fnc_arr[GLOBAL_ERROR](BOARD_EMPTY_SQUARE, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[BOARD_EMPTY_SQUARE](BOARD_EMPTY_SQUARE,
				"In file " FILE_NAME ", " FUNC_NAME);

	}

	#undef FUNC_NAME

	validate_move(b, r_old, c_old, r_new, c_new);

	if(b->board_arr[r_new][c_new]){
		if(b->b_capture_list && b->w_capture_list){
			board_remove_piece(b, r_new, c_new, false);
		}else{
			board_remove_piece(b, r_new, c_new, true);
		}
	}
	piece_set_pos(b->board_arr[r_old][c_old], r_new, c_new);
	b->board_arr[r_new][c_new] = b->board_arr[r_old][c_old];
	b->board_arr[r_old][c_old] = NULL;
}

Piece board_get_piece(Board b, uint_fast8_t r, uint_fast8_t c)
{
	#define FUNC_NAME "const Piece board_get_piece(Board b, "\
		"uint_fast8_t r, uint_fast8_t c)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file " FILE_NAME
				", " FUNC_NAME);

	}else if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return b->board_arr[r][c];
}

uint_fast8_t board_get_size(void)
{

	return BOARD_SIZE;
}

void board_set_turn(Board b, enum PieceColor c)
{
	#define FUNC_NAME "void board_set_turn(Board b, uint_fast8_t)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME
			", " FUNC_NAME);

	}

	if(c != BLACK && c != WHITE){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file" FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	*(b->turn) = c;
}

void board_flip_turn(Board b)
{
	#define FUNC_NAME "void board_flip_turn(Board b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME 
				", " FUNC_NAME);
	}

	#undef FUNC_NAME

	*(b->turn) == WHITE ? (*(b->turn) = BLACK) : (*(b->turn) = WHITE);
}

enum PieceColor board_get_turn(const Board b)
{
	#define FUNC_NAME "uint_fast8_t board_get_turn(Board b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME 
				", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return *(b->turn);
}

Piece board_remove_piece(Board b, uint_fast8_t r, uint_fast8_t c, bool destroy)
{
	#define FUNC_NAME "Piece board_remove_piece(Board b, uint_fast8_t r, "\
		"uint_fast8_t c, bool destroy)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file " FILE_NAME
				", " FUNC_NAME);

	}else if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);

	}else if(!b->board_arr[r][c]){
		if(!err_fnc_arr[BOARD_EMPTY_SQUARE])
			err_fnc_arr[GLOBAL_ERROR](BOARD_EMPTY_SQUARE, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[BOARD_EMPTY_SQUARE](BOARD_EMPTY_SQUARE,
				"In file" FILE_NAME ", " FUNC_NAME);
	}

	if(destroy){
		if(b->b_capture_list || b->w_capture_list){
			if(!err_fnc_arr[CONFLICTING_PARAM])
				err_fnc_arr[GLOBAL_ERROR](CONFLICTING_PARAM,
					"In file " FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[CONFLICTING_PARAM](CONFLICTING_PARAM
					, "In file " FILE_NAME ", " FUNC_NAME);
		}
		piece_destroy(&b->board_arr[r][c]);
		b->board_arr[r][c] = NULL;
		return NULL;
	}else{
		if(b->b_capture_list || b->w_capture_list){
			if(piece_get_color(b->board_arr[r][c]) == WHITE){
				for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
					if(!b->w_capture_list[i]){
						b->w_capture_list[i] =
						b->board_arr[r][c];
						b->board_arr[r][c] = NULL;
						return b->w_capture_list[i];
					}
			}else{
				for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
					if(!b->b_capture_list[i]){
						b->b_capture_list[i] =
						b->board_arr[r][c];
						b->board_arr[r][c] = NULL;
						return b->b_capture_list[i];
					}
			}
		}
		Piece tmp = b->board_arr[r][c];
		b->board_arr[r][c] = NULL;
		return tmp;
	}

	#undef FUNC_NAME
}

void board_init_capture_list(Board b)
{
	#define FUNC_NAME "void board_init_capture_list(Board b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file" FILE_NAME 
				", " FUNC_NAME);
	}

	b->b_capture_list = calloc(PIECE_COUNT, sizeof(Piece));
	if(!b->b_capture_list){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file" FILE_NAME 
				", " FUNC_NAME);
	}
	
	b->w_capture_list = calloc(PIECE_COUNT, sizeof(Piece));
	if(!b->w_capture_list){
		if(!err_fnc_arr[MEM_FAIL])
			err_fnc_arr[GLOBAL_ERROR](MEM_FAIL, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[MEM_FAIL](MEM_FAIL, "In file" FILE_NAME 
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

Piece *board_get_capture_list(const Board b, enum PieceColor c)
{
	#define FUNC_NAME "Piece *board_get_capture_list(Board b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(c != WHITE && c != BLACK){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file" FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return c == WHITE ? b->w_capture_list : b->b_capture_list;
}

void board_set_piece_moved(const Board b, enum UnmovedPieces p, bool moved)
{
	#define FUNC_NAME "void board_set_piece_moved(const Board b, "\
		"enum UnmovedPieces p, bool moved)"
	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(p != W_L_ROOK && p != W_R_ROOK && p != W_KING &&
		p != B_L_ROOK && p != B_R_ROOK && p != B_KING){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	b->unmoved_pieces[p] = moved;
}

bool board_has_piece_moved(const Board b, enum UnmovedPieces p)
{
	#define FUNC_NAME "bool board_has_piece_moved(const Board b, "\
		"enum UnmovedPieces p"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(p != W_L_ROOK && p != W_R_ROOK && p != W_KING &&
		p != B_L_ROOK && p != B_R_ROOK && p != B_KING){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return b->unmoved_pieces[p];
}

void board_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg))
{
	#define FUNC_NAME "void gl_set_err_hndl(enum ErrorCode error_type, "\
		"void (*err_hndl)(enum ErrorCode err, const char *msg))"

	if(!(
		error_type != MEM_FAIL
		||
		error_type != GLOBAL_ERROR
		||
		error_type != NULL_PARAM
		||
		error_type != CONFLICTING_PARAM
		||
		error_type != INVALID_ENUM_PARAM
		||
		error_type != INVALID_INT_PARAM
		||
		error_type != BOARD_NONEMPTY_SQUARE
		||
		error_type != BOARD_EMPTY_SQUARE
		||
		error_type != PIECE_MOVE_SAME_POS
	))
		err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	err_fnc_arr[error_type] = err_hndl;
}
