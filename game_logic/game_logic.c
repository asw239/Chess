#include "game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>

#define FILE_NAME "game_logic.c"

static void check_move_same_pos(const Piece p, uint_fast8_t x, uint_fast8_t y);
static void check_ally_overlap(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y);
static void check_move_king(const Piece p, uint_fast8_t x, uint_fast8_t y);
static void check_move_rook(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y, bool checkQueen, bool *in_range);
static void check_move_bishop(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y, bool checkQueen, bool *in_range);
static void check_move_queen(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y);
static void check_move_knight(const Piece p, uint_fast8_t x, uint_fast8_t y);
static void check_move_pawn(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y);

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

Board generate_start_board(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);

	Piece **board_arr = board_get_arr(b);
	Piece tmp; 

	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 0);
	board_arr[0][0] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 7);
	board_arr[0][7] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 0);
	board_arr[7][0] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 7);
	board_arr[7][7] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 1);
	board_arr[0][1] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 6);
	board_arr[0][6] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 1);
	board_arr[7][1] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 6);
	board_arr[7][6] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 2);
	board_arr[0][2] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 5);
	board_arr[0][5] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 2);
	board_arr[7][2] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 5);
	board_arr[7][5] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 3);
	board_arr[0][3] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 3);
	board_arr[7][3] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, BLACK);
	piece_set_pos(tmp, 0, 4);
	board_arr[0][4] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, WHITE);
	piece_set_pos(tmp, 7, 4);
	board_arr[7][4] = tmp;

	for(int i = 1; i < board_get_size(); i+=5){
		for(int j = 0; j < board_get_size(); j++){
			tmp = piece_create();
			piece_set_type(tmp, pawn);
			i == 1
				?piece_set_color(tmp, BLACK)
				:piece_set_color(tmp, WHITE);
			piece_set_pos(tmp, i, j);
			board_arr[i][j] = tmp;
		}
	}

	return b;
}

void validate_move(const Board b, const Piece p, uint_fast8_t x, uint_fast8_t y)
{
	#define FUNC_NAME "bool validate_move(const Board b, const Piece p, "\
		"uint_fast8_t x, uint_fast8_t y)"

	if(!b || !p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(x >= board_get_size() || y >= board_get_size()){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	bool found = false;
	for(uint_fast8_t i = 0; i < board_get_size(); i++)
		for(uint_fast8_t j = 0; j < board_get_size(); j++)
			if(board_get_arr(b)[i][j] == p)
				found = true;
	if(!found){
		if(!err_fnc_arr[PIECE_NOT_IN_BOARD])
			err_fnc_arr[GLOBAL_ERROR](PIECE_NOT_IN_BOARD,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_NOT_IN_BOARD](PIECE_NOT_IN_BOARD,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	check_move_same_pos(p, x, y);
	check_ally_overlap(b, p, x, y);

	switch(piece_get_type(p)){
	case king:
		check_move_king(p, x, y);
		break;
	case rook:
		check_move_rook(b, p, x, y, false, NULL);
		break;
	case bishop:
		check_move_bishop(b, p, x, y, false, NULL);
		break;
	case queen:
		check_move_queen(b, p, x, y);
		break;
	case knight:
		check_move_knight(p, x, y);
		break;
	case pawn:
		check_move_pawn(b, p, x, y);
		break;
	}
}

static void check_move_same_pos(const Piece p, uint_fast8_t x, uint_fast8_t y)
{
	#define FUNC_NAME "static void check_move_same_pos(const Piece p, "\
		"uint_fast8_t x, uint_fast8_t y)"

	if(piece_get_x(p) == x && piece_get_y(p) == y){
		if(!err_fnc_arr[PIECE_MOVE_SAME_POS])
			err_fnc_arr[GLOBAL_ERROR](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[PIECE_MOVE_SAME_POS](PIECE_MOVE_SAME_POS,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME
}

static void check_ally_overlap(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y)
{
	#define FUNC_NAME "static void check_ally_overlap(const Board b, " \
		"const Piece p, uint_fast8_t x, uint_fast8_t y)"

	if(
		board_get_arr(b)[x][y]
		&&
		piece_get_color(p) == piece_get_color(board_get_arr(b)[x][y])
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
}

static void check_move_king(const Piece p, uint_fast8_t x, uint_fast8_t y)
{
	#define FUNC_NAME "static void check_move_king(const Piece p, " \
		"uint_fast8_t x, uint_fast8_t y)"

	if(
		piece_get_x(p) - 1 > x || piece_get_x(p) + 1 < x
		||
		piece_get_y(p) - 1 > y || piece_get_y(p) + 1 < y
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

static void check_move_rook(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y, bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_rook(const Board b, "\
		"const Piece p, uint_fast8_t x, uint_fast8_t y, "\
		"bool checkQueen, bool *in_range)"

	//vertical check: bottom -> top
	if(piece_get_y(p) == y && piece_get_x(p) < x){
		for(uint_fast8_t i = piece_get_x(p) + 1; i < x; i++){
			if(board_get_arr(b)[i][y]){
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
	}else if(piece_get_y(p) == y && piece_get_x(p) > x){
		for(uint_fast8_t i = piece_get_x(p) - 1; i > x; i--){
			if(board_get_arr(b)[i][y]){
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
	}else if(piece_get_x(p) == x && piece_get_y(p) < y){
		for(uint_fast8_t i = piece_get_y(p) + 1; i < y; i++){
			if(board_get_arr(b)[x][i]){
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
	}else if(piece_get_x(p) == x && piece_get_y(p) > y){
		for(uint_fast8_t i = piece_get_y(p) - 1; i > y; i--){
			if(board_get_arr(b)[x][i]){
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

static void check_move_bishop(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y, bool checkQueen, bool *in_range)
{
	if(checkQueen)
		*in_range = true;

	#define FUNC_NAME "static void check_move_bishop(const Board b, "\
		"const Piece p, uint_fast8_tx, uint_fast8_t y, "\
		"bool checkQueen, bool *in_range)"

	//bottom-left -> top-right
	if(	piece_get_x(p) - x < 0
		&&
		piece_get_y(p) - y < 0
		&&
		piece_get_x(p) - x == piece_get_y(p) - y
	){
		for(uint_fast8_t i = piece_get_x(p) + 1, j = piece_get_y(p) + 1;
			i < x && j < y;
			i++, j++){
			if(board_get_arr(b)[i][j]){
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
		piece_get_x(p) - x > 0
		&&
		piece_get_y(p) - y < 0
		&&
		piece_get_x(p) - x == y - piece_get_y(p)
	){
		for(uint_fast8_t i = piece_get_x(p) - 1, j = piece_get_y(p) + 1;
			i > x && j < y;
			i--, j++){
			if(board_get_arr(b)[i][j]){
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
		piece_get_x(p) - x > 0
		&&
		piece_get_y(p) - y > 0
		&&
		piece_get_x(p) - x == piece_get_y(p) - y
	){
		for(uint_fast8_t i = piece_get_x(p) - 1, j = piece_get_y(p) - 1;
			i > x && j > y;
			i--, j--){
			if(board_get_arr(b)[i][j]){
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
		piece_get_x(p) - x < 0
		&&
		piece_get_y(p) - y > 0
		&&
		x - piece_get_x(p) == piece_get_y(p) - y
	){
		for(uint_fast8_t i = piece_get_x(p) + 1, j = piece_get_y(p) - 1;
			i < x && j > y;
			i++, j--){
			if(board_get_arr(b)[i][j]){
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

static void check_move_queen(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y)
{
	bool bishop_in_range;
	bool rook_in_range;

	#define FUNC_NAME "static void check_move_queen(const Board b, "\
		"const Piece p, uint_fast8_t x, uint_fast8_t y)"

	check_move_bishop(b, p, x, y, true, &bishop_in_range);
	check_move_rook(b, p, x, y, true, &rook_in_range);
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

static void check_move_knight(const Piece p, uint_fast8_t x, uint_fast8_t y)
{
	#define FUNC_NAME "static void check_move_knight(const Board b, "\
		"const Piece p, uint_fast8_t x, uint_fast8_t y)"

	if(!(
		(x - piece_get_x(p) == 2 && y - piece_get_y(p) == 1)
		||
		(x - piece_get_x(p) == 1 && y - piece_get_y(p) == 2)
		||
		(x - piece_get_x(p) == -1 && y - piece_get_y(p) == 2)
		||
		(x - piece_get_x(p) == -2 && y - piece_get_y(p) == 1)
		||
		(x - piece_get_x(p) == -2 && y - piece_get_y(p) == -1)
		||
		(x - piece_get_x(p) == -1 && y - piece_get_y(p) == -2)
		||
		(x - piece_get_x(p) == 1 && y - piece_get_y(p) == -2)
		||
		(x - piece_get_x(p) == 2 && y - piece_get_y(p) == -1)
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

static void check_move_pawn(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y)
{
	#define FUNC_NAME "static void check_move_pawn(const Board b, "\
		"const Piece p, uint_fast8_t x, uint_fast8_t y)"

	//black forward
	if(
		x - piece_get_x(p) == 1
		&&
		y - piece_get_y(p) == 0
		&&
		piece_get_color(p) == BLACK
	){
		if(board_get_arr(b)[x][y]){
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
		x - piece_get_x(p) == -1
		&&
		y - piece_get_y(p) == 0
		&&
		piece_get_color(p) == WHITE
	){
		if(board_get_arr(b)[x][y]){
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
		x - piece_get_x(p) == 2
		&&
		y - piece_get_y(p) == 0
		&&
		piece_get_color(p) == BLACK
		&&
		piece_get_x(p) == 1
	){
		if(board_get_arr(b)[x][y] || board_get_arr(b)[x - 1][y]){
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
		x - piece_get_x(p) == -2
		&&
		y - piece_get_y(p) == 0
		&&
		piece_get_color(p) == WHITE
		&&
		piece_get_x(p) ==  6
	){
		if(board_get_arr(b)[x][y] || board_get_arr(b)[x + 1][y]){
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
		x - piece_get_x(p) == 1
		&&
		y - piece_get_y(p) >= -1 && y - piece_get_y(p) <= 1
		&&
		piece_get_color(p) == BLACK
		&&
		board_get_arr(b)[x][y]
		&&
		piece_get_color(board_get_arr(b)[x][y]) == WHITE
	){
	//white capture
	}else if(
		x - piece_get_x(p) == -1
		&&
		y - piece_get_y(p) >= -1 && y - piece_get_y(p) <= 1
		&&
		piece_get_color(p) == WHITE
		&&
		board_get_arr(b)[x][y]
		&&
		piece_get_color(board_get_arr(b)[x][y]) == BLACK
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
