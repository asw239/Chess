#include "game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>

#define FILE_NAME "game_logic.c"

static void check_move_king(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool checkQueen, bool *in_range);
static void check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool checkQueen, bool *in_range);
static void check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

Board generate_start_board(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);

	Piece tmp; 

	tmp = piece_create();
	piece_set_type(tmp, ROOK);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 0);
	tmp = piece_create();
	piece_set_type(tmp, ROOK);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 7);
	tmp = piece_create();
	piece_set_type(tmp, ROOK);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 0);
	tmp = piece_create();
	piece_set_type(tmp, ROOK);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 7);

	tmp = piece_create();
	piece_set_type(tmp, KNIGHT);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 1);
	tmp = piece_create();
	piece_set_type(tmp, KNIGHT);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 6);
	tmp = piece_create();
	piece_set_type(tmp, KNIGHT);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 1);
	tmp = piece_create();
	piece_set_type(tmp, KNIGHT);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 6);

	tmp = piece_create();
	piece_set_type(tmp, BISHOP);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 2);
	tmp = piece_create();
	piece_set_type(tmp, BISHOP);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 5);
	tmp = piece_create();
	piece_set_type(tmp, BISHOP);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 2);
	tmp = piece_create();
	piece_set_type(tmp, BISHOP);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 5);

	tmp = piece_create();
	piece_set_type(tmp, QUEEN);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 4);
	tmp = piece_create();
	piece_set_type(tmp, QUEEN);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 4);

	tmp = piece_create();
	piece_set_type(tmp, KING);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 3);
	tmp = piece_create();
	piece_set_type(tmp, KING);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 3);

	for(int i = 1; i < board_get_size(); i+=5){
		for(int j = 0; j < board_get_size(); j++){
			tmp = piece_create();
			piece_set_type(tmp, PAWN);
			i == 1
				?piece_set_color(tmp, BLACK)
				:piece_set_color(tmp, WHITE);
			board_link_piece(b, tmp, i, j);
		}
	}

	return b;
}

void validate_move(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	#define FUNC_NAME "void validate_move(const Board b, "\
		"uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, "\
		"uint_fast8 c_new)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);

	}else if(r_old >= board_get_size() || c_old >= board_get_size() ||
		r_new >= board_get_size() || c_new >= board_get_size()){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
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

	}else if(!board_get_piece(b, r_old, c_old)){
		if(!err_fnc_arr[BOARD_EMPTY_SQUARE])
			err_fnc_arr[GLOBAL_ERROR](BOARD_EMPTY_SQUARE, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[BOARD_EMPTY_SQUARE](BOARD_EMPTY_SQUARE,
				"In file " FILE_NAME ", " FUNC_NAME);
	}else if(
		board_get_piece(b, r_new, c_new)
		&&
		piece_get_color(board_get_piece(b, r_new, c_new))
		==
		piece_get_color(board_get_piece(b, r_old, c_old))
	){
		if(!err_fnc_arr[PIECE_MOVE_OVERLAPS_ALLY])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_OVERLAPS_ALLY,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_OVERLAPS_ALLY](
				PIECE_MOVE_OVERLAPS_ALLY, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME

	switch(piece_get_type(board_get_piece(b, r_old, c_old))){
	case KING:
		check_move_king(r_old, c_old, r_new, c_new);
		break;
	case ROOK:
		check_move_rook(b, r_old, c_old, r_new, c_new, false, NULL);
		break;
	case BISHOP:
		check_move_bishop(b, r_old, c_old, r_new, c_new, false, NULL);
		break;
	case QUEEN:
		check_move_queen(b, r_old, c_old, r_new, c_new);
		break;
	case KNIGHT:
		check_move_knight(r_old, c_old, r_new, c_new);
		break;
	case PAWN:
		check_move_pawn(b, r_old, c_old, r_new, c_new);
		break;
	}
}

static void check_move_king(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	#define FUNC_NAME "static void check_move_king(uint_fast8_t r_old, "\
		"uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)"

	if(
		r_old - 1 > r_new || r_old + 1 < r_new
		||
		c_old - 1 > c_new || c_old + 1 < c_new
	){
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

static void check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_rook(const Board b, "\
		"uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, "\
		"uint_fast8_t y_new, bool checkQueen, bool *in_range)"

	//vertical check: bottom -> top
	if(c_old == c_new && r_old < r_new){
		for(uint_fast8_t i = r_old + 1; i < r_new; i++){
			if(board_get_piece(b, i, c_old)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//vertical check: top -> bottom
	}else if(c_old == c_new && r_old > r_new){
		for(uint_fast8_t i = r_old - 1; i > r_new; i--){
			if(board_get_piece(b, i, c_old)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//horizontal check: left -> right
	}else if(r_old == r_new && c_old < c_new){
		for(uint_fast8_t i = c_old + 1; i < c_new; i++){
			if(board_get_piece(b, r_old, i)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//horizontal check: right -> left
	}else if(r_old == r_new && c_old > c_new){
		for(uint_fast8_t i = c_old - 1; i > c_new; i--){
			if(board_get_piece(b, r_old, i)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//illegal move
	}else{
		if(checkQueen){
			*in_range = false;
			return ;
		}
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

static void check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_bishop(const Board b, "\
		"uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, "\
		"uint_fast8_t c_new, bool checkQueen, bool *in_range)"

	//bottom-left -> top-right
	if(	r_old - r_new < 0
		&&
		c_old - c_new < 0
		&&
		r_old - r_new == c_old - c_new
	){
		for(uint_fast8_t i = r_old + 1, j = c_old + 1;
			i < r_new && j < c_new;
			i++, j++){
			if(board_get_piece(b, i, j)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//top-left -> bottom right
	}else if(
		r_old - r_new > 0
		&&
		c_old - c_new < 0
		&&
		r_old - r_new == c_new - c_old
	){
		for(uint_fast8_t i = r_old - 1, j = c_old + 1;
			i > r_new && j < c_new;
			i--, j++){
			if(board_get_piece(b, i, j)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}

			}
		}
	//top-right -> bottom-left
	}else if(
		r_old - r_new > 0
		&&
		c_old - c_new > 0
		&&
		r_old - r_new == c_old - c_new
	){
		for(uint_fast8_t i = r_old - 1, j = c_old - 1;
			i > r_new && j > c_new;
			i--, j--){
			if(board_get_piece(b, i, j)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//bottom-right -> top-left
	}else if(
		r_old - r_new < 0
		&&
		c_old - c_new > 0
		&&
		r_new - r_old == c_old - c_new
	){
		for(uint_fast8_t i = r_old + 1, j = c_old - 1;
			i < r_new && j > c_new;
			i++, j--){
			if(board_get_piece(b, i, j)){
				if(!err_fnc_arr[PIECE_MOVE_COLLISION]){
					err_fnc_arr[GLOBAL_ERROR](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}else{
					err_fnc_arr[PIECE_MOVE_COLLISION](
						PIECE_MOVE_COLLISION, "In file "
						FILE_NAME ", " FUNC_NAME);
				}
			}
		}
	//illegal move
	}else{
		if(checkQueen){
			*in_range = false;
			return ;
		}
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

static void check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	bool bishop_in_range;
	bool rook_in_range;

	#define FUNC_NAME "static void check_move_queen(const Board b, "\
		"uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, "\
		"uint_fast8_t c_new)"

	check_move_bishop(b, r_old, c_old, r_new, c_new, true,
		&bishop_in_range);
	check_move_rook(b, r_old, c_old, r_new, c_new, true, &rook_in_range);
	if(!(bishop_in_range || rook_in_range)){
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

static void check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	#define FUNC_NAME "static void check_move_knight(uint_fast8_t r_old, "\
		"uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)"

	if(!(
		(r_new - r_old == 2 && c_new - c_old == 1)
		||
		(r_new - r_old == 1 && c_new - c_old == 2)
		||
		(r_new - r_old == -1 && c_new - c_old == 2)
		||
		(r_new - r_old == -2 && c_new - c_old == 1)
		||
		(r_new - r_old == -2 && c_new - c_old == -1)
		||
		(r_new - r_old == -1 && c_new - c_old == -2)
		||
		(r_new - r_old == 1 && c_new - c_old == -2)
		||
		(r_new - r_old == 2 && c_new - c_old == -1)
	)){
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, "In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME

}

static void check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	#define FUNC_NAME "static void check_move_pawn(const Board b, "\
		"uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, "\
		"uint_fast8_t c_new)"

	//black forward
	if(
		r_new - r_old == 1
		&&
		c_new - c_old == 0
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == BLACK
	){
		if(board_get_piece(b, r_new, c_new)){
			if(!err_fnc_arr[PIECE_MOVE_COLLISION])
				err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_COLLISION,
					"In file " FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[PIECE_MOVE_COLLISION](
					PIECE_MOVE_COLLISION, "In file "
					FILE_NAME ", " FUNC_NAME);
		}
	//white forward
	}else if(
		r_new - r_old == -1
		&&
		c_new - c_old == 0
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == WHITE
	){
		if(board_get_piece(b, r_new, c_new)){
			if(!err_fnc_arr[PIECE_MOVE_COLLISION])
				err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_COLLISION,
					"In file " FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[PIECE_MOVE_COLLISION](
					PIECE_MOVE_COLLISION, "In file "
					FILE_NAME ", " FUNC_NAME);
		}
	//black double forward
	}else if(
		r_new - r_old == 2
		&&
		c_new - c_old == 0
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == BLACK
		&&
		r_old == 1
	){
		if(
			board_get_piece(b, r_new, c_new)
			||
			board_get_piece(b, r_new - 1, c_new)
		){
			if(!err_fnc_arr[PIECE_MOVE_COLLISION])
				err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_COLLISION,
					"In file " FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[PIECE_MOVE_COLLISION](
					PIECE_MOVE_COLLISION, "In file "
					FILE_NAME ", " FUNC_NAME);
		}
	//white double forward
	}else if(
		r_new - r_old == -2
		&&
		c_new - c_old == 0
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == WHITE
		&&
		r_old ==  6
	){
		if(
			board_get_piece(b, r_new, c_new)
			||
			board_get_piece(b, r_new + 1, c_new)
		){
			if(!err_fnc_arr[PIECE_MOVE_COLLISION])
				err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_COLLISION,
					"In file " FILE_NAME ", " FUNC_NAME);
			else
				err_fnc_arr[PIECE_MOVE_COLLISION](
					PIECE_MOVE_COLLISION, "In file "
					FILE_NAME ", " FUNC_NAME);
		}
	//black capture
	}else if(
		r_new - r_old == 1
		&&
		c_new - c_old >= -1 && c_new - c_old <= 1
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == BLACK
		&&
		board_get_piece(b, r_new, c_new)
		&&
		piece_get_color(board_get_piece(b, r_new, c_new)) == WHITE
	){
	//white capture
	}else if(
		r_new - r_old == -1
		&&
		c_new - c_old >= -1 && c_new - c_old <= 1
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == WHITE
		&&
		board_get_piece(b, r_new, c_new)
		&&
		piece_get_color(board_get_piece(b, r_new, c_new)) == BLACK
	){
	//illegal move
	}else{
		if(!err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_NOT_IN_RANGE,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_NOT_IN_RANGE](
				PIECE_MOVE_NOT_IN_RANGE, " In file " FILE_NAME
				", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

void gl_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg))
{
	#define FUNC_NAME "void gl_set_err_hndl(enum ErrorCode error_type, "\
		"void (*err_hndl)(enum ErrorCode err, const char *msg))"

	if(!(
		error_type != GLOBAL_ERROR
		||
		error_type != NULL_PARAM
		||
		error_type != INVALID_INT_PARAM
		||
		error_type != PIECE_NOT_IN_BOARD
		||
		error_type != PIECE_MOVE_SAME_POS
		||
		error_type != PIECE_MOVE_OVERLAPS_ALLY
		||
		error_type != PIECE_MOVE_NOT_IN_RANGE
		||
		error_type != BOARD_EMPTY_SQUARE
		||
		error_type != PIECE_MOVE_COLLISION
	))
		err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	err_fnc_arr[error_type] = err_hndl;
}
