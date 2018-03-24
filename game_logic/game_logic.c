#include "game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>

#define FILE_NAME "game_logic.c"

static void check_move_king(uint_fast8_t x_old, uint_fast8_t y_old,
	uint_fast8_t x_new, uint_fast8_t y_new);
static void check_move_rook(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new,
	bool checkQueen, bool *in_range);
static void check_move_bishop(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new,
	bool checkQueen, bool *in_range);
static void check_move_queen(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new);
static void check_move_knight(uint_fast8_t x_old, uint_fast8_t y_old,
	uint_fast8_t x_new, uint_fast8_t y_new);
static void check_move_pawn(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new);

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

Board generate_start_board(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);

	Piece tmp; 

	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 0);
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 7);
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 0);
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 7);

	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 1);
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 6);
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 1);
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 6);

	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 2);
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 5);
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 2);
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 5);

	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 4);
	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 4);

	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, BLACK);
	board_link_piece(b, tmp, 0, 3);
	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, WHITE);
	board_link_piece(b, tmp, 7, 3);

	for(int i = 1; i < board_get_size(); i+=5){
		for(int j = 0; j < board_get_size(); j++){
			tmp = piece_create();
			piece_set_type(tmp, pawn);
			i == 1
				?piece_set_color(tmp, BLACK)
				:piece_set_color(tmp, WHITE);
			board_link_piece(b, tmp, i, j);
		}
	}

	return b;
}

void validate_move(const Board b, uint_fast8_t x_old, uint_fast8_t y_old,
	uint_fast8_t x_new, uint_fast8_t y_new)
{
	#define FUNC_NAME "void validate_move(const Board b, "\
		"uint_fast8_t x_old, uint_fast8_t y_old, uint_fast8_t x_new, "\
		"uint_fast8 y_new)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);

	}else if(x_old >= board_get_size() || y_old >= board_get_size() ||
		x_new >= board_get_size() || y_new >= board_get_size()){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);

	}else if(x_old == x_new && y_old == y_new){
		if(!err_fnc_arr[PIECE_MOVE_SAME_POS])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_SAME_POS](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);

	}else if(!board_get_piece(b, x_old, y_old)){
		if(!err_fnc_arr[BOARD_EMPTY_SQUARE])
			err_fnc_arr[GLOBAL_ERROR](BOARD_EMPTY_SQUARE, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[BOARD_EMPTY_SQUARE](BOARD_EMPTY_SQUARE,
				"In file " FILE_NAME ", " FUNC_NAME);
	}else if(
		board_get_piece(b, x_new, y_new)
		&&
		piece_get_color(board_get_piece(b, x_new, y_new))
		==
		piece_get_color(board_get_piece(b, x_old, y_old))
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

	switch(piece_get_type(board_get_piece(b, x_old, y_old))){
	case king:
		check_move_king(x_old, y_old, x_new, y_new);
		break;
	case rook:
		check_move_rook(b, x_old, y_old, x_new, y_new, false, NULL);
		break;
	case bishop:
		check_move_bishop(b, x_old, y_old, x_new, y_new, false, NULL);
		break;
	case queen:
		check_move_queen(b, x_old, y_old, x_new, y_new);
		break;
	case knight:
		check_move_knight(x_old, y_old, x_new, y_new);
		break;
	case pawn:
		check_move_pawn(b, x_old, y_old, x_new, y_new);
		break;
	}
}

static void check_move_king(uint_fast8_t x_old, uint_fast8_t y_old,
	uint_fast8_t x_new, uint_fast8_t y_new)
{
	#define FUNC_NAME "static void check_move_king(uint_fast8_t x_old, "\
		"uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new)"

	if(
		x_old - 1 > x_new || x_old + 1 < x_new
		||
		y_old - 1 > y_new || y_old + 1 < y_new
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

static void check_move_rook(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new,
	bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_rook(const Board b, "\
		"uint_fast8_t x_old, uint_fast8_t y_old, uint_fast8_t x_new, "\
		"uint_fast8_t y_new, bool checkQueen, bool *in_range)"

	//vertical check: bottom -> top
	if(y_old == y_new && x_old < x_new){
		for(uint_fast8_t i = x_old + 1; i < x_new; i++){
			if(board_get_piece(b, i, y_old)){
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
	}else if(y_old == y_new && x_old > x_new){
		for(uint_fast8_t i = x_old - 1; i > x_new; i--){
			if(board_get_piece(b, i, y_old)){
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
	}else if(x_old == x_new && y_old < y_new){
		for(uint_fast8_t i = y_old + 1; i < y_new; i++){
			if(board_get_piece(b, x_old, i)){
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
	}else if(x_old == x_new && y_old > y_new){
		for(uint_fast8_t i = y_old - 1; i > y_new; i--){
			if(board_get_piece(b, x_old, i)){
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

static void check_move_bishop(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new,
	bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_bishop(const Board b, "\
		"uint_fast8_t x_old, uint_fast8_t y_old, uint_fast8_t x_new, "\
		"uint_fast8_t y_new, bool checkQueen, bool *in_range)"

	//bottom-left -> top-right
	if(	x_old - x_new < 0
		&&
		y_old - y_new < 0
		&&
		x_old - x_new == y_old - y_new
	){
		for(uint_fast8_t i = x_old + 1, j = y_old + 1;
			i < x_new && j < y_new;
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
		x_old - x_new > 0
		&&
		y_old - y_new < 0
		&&
		x_old - x_new == y_new - y_old
	){
		for(uint_fast8_t i = x_old - 1, j = y_old + 1;
			i > x_new && j < y_new;
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
		x_old - x_new > 0
		&&
		y_old - y_new > 0
		&&
		x_old - x_new == y_old - y_new
	){
		for(uint_fast8_t i = x_old - 1, j = y_old - 1;
			i > x_new && j > y_new;
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
		x_old - x_new < 0
		&&
		y_old - y_new > 0
		&&
		x_new - x_old == y_old - y_new
	){
		for(uint_fast8_t i = x_old + 1, j = y_old - 1;
			i < x_new && j > y_new;
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

static void check_move_queen(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new)
{
	bool bishop_in_range;
	bool rook_in_range;

	#define FUNC_NAME "static void check_move_queen(const Board b, "\
		"uint_fast8_t x_old, uint_fast8_t y_old, uint_fast8_t x_new, "\
		"uint_fast8_t y_new)"

	check_move_bishop(b, x_old, y_old, x_new, y_new, true,
		&bishop_in_range);
	check_move_rook(b, x_old, y_old, x_new, y_new, true, &rook_in_range);
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

static void check_move_knight(uint_fast8_t x_old, uint_fast8_t y_old,
	uint_fast8_t x_new, uint_fast8_t y_new)
{
	#define FUNC_NAME "static void check_move_knight(uint_fast8_t x_old, "\
		"uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new)"

	if(!(
		(x_new - x_old == 2 && y_new - y_old == 1)
		||
		(x_new - x_old == 1 && y_new - y_old == 2)
		||
		(x_new - x_old == -1 && y_new - y_old == 2)
		||
		(x_new - x_old == -2 && y_new - y_old == 1)
		||
		(x_new - x_old == -2 && y_new - y_old == -1)
		||
		(x_new - x_old == -1 && y_new - y_old == -2)
		||
		(x_new - x_old == 1 && y_new - y_old == -2)
		||
		(x_new - x_old == 2 && y_new - y_old == -1)
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

static void check_move_pawn(const Board b, uint_fast8_t x_old,
	uint_fast8_t y_old, uint_fast8_t x_new, uint_fast8_t y_new)
{
	#define FUNC_NAME "static void check_move_pawn(const Board b, "\
		"uint_fast8_t x_old, uint_fast8_t y_old, uint_fast8_t x_new, "\
		"uint_fast8_t y_new)"

	//black forward
	if(
		x_new - x_old == 1
		&&
		y_new - y_old == 0
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == BLACK
	){
		if(board_get_piece(b, x_new, y_new)){
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
		x_new - x_old == -1
		&&
		y_new - y_old == 0
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == WHITE
	){
		if(board_get_piece(b, x_new, y_new)){
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
		x_new - x_old == 2
		&&
		y_new - y_old == 0
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == BLACK
		&&
		x_old == 1
	){
		if(
			board_get_piece(b, x_new, y_new)
			||
			board_get_piece(b, x_new - 1, y_new)
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
		x_new - x_old == -2
		&&
		y_new - y_old == 0
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == WHITE
		&&
		x_old ==  6
	){
		if(
			board_get_piece(b, x_new, y_new)
			||
			board_get_piece(b, x_new + 1, y_new)
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
		x_new - x_old == 1
		&&
		y_new - y_old >= -1 && y_new - y_old <= 1
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == BLACK
		&&
		board_get_piece(b, x_new, y_new)
		&&
		piece_get_color(board_get_piece(b, x_new, y_new)) == WHITE
	){
	//white capture
	}else if(
		x_new - x_old == -1
		&&
		y_new - y_old >= -1 && y_new - y_old <= 1
		&&
		piece_get_color(board_get_piece(b, x_old, y_old)) == WHITE
		&&
		board_get_piece(b, x_new, y_new)
		&&
		piece_get_color(board_get_piece(b, x_new, y_new)) == BLACK
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
	))
		err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	err_fnc_arr[error_type] = err_hndl;
}
