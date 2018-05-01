#include "game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>

static void check_move_king(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision);
static void check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision);
static void check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);

static ErrFncPtr err_fnc_arr[ERROR_CODE_COUNT] = {[GLOBAL_ERROR] = def_hndl};
static const char *FILE_NAME = "game_logic.c";

Board generate_start_board(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);

	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

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

	for(int i = 1; i < BOARD_SIZE; i+=5){
		for(int j = 0; j < BOARD_SIZE; j++){
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
	static const char *FUNC_NAME = "void validate_move(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(r_old >= BOARD_SIZE || c_old >= BOARD_SIZE ||
		r_new >= BOARD_SIZE || c_new >= BOARD_SIZE){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	if(r_old == r_new && c_old == c_new){
		call_error(err_fnc_arr, PIECE_MOVE_SAME_POS, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	if(!board_get_piece(b, r_old, c_old)){
		call_error(err_fnc_arr, BOARD_EMPTY_SQUARE, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	if(
		board_get_piece(b, r_new, c_new)
		&&
		piece_get_color(board_get_piece(b, r_new, c_new))
		==
		piece_get_color(board_get_piece(b, r_old, c_old))
	){
		call_error(err_fnc_arr, PIECE_MOVE_SAME_POS, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	switch(piece_get_type(board_get_piece(b, r_old, c_old))){
	case KING:
		check_move_king(r_old, c_old, r_new, c_new);
		break;
	case ROOK:
		check_move_rook(b, r_old, c_old, r_new, c_new, false, NULL,
			NULL);
		break;
	case BISHOP:
		check_move_bishop(b, r_old, c_old, r_new, c_new, false, NULL,
			NULL);
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
	const char *FUNC_NAME = "static void check_move_king(\
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

	if(
		r_old - 1 > r_new || r_old + 1 < r_new
		||
		c_old - 1 > c_new || c_old + 1 < c_new
	){
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

static void check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision)
{
	const char *FUNC_NAME = "static void check_move_rook(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new, bool check_queen, bool *in_range, bool *collision)";

	if(check_queen){
		*in_range = true;
		*collision = false;
	}

	//vertical check: bottom -> top
	if(c_old == c_new && r_old < r_new){
		for(uint_fast8_t i = r_old + 1; i < r_new; i++){
			if(board_get_piece(b, i, c_old)){
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
			}
		}
	//vertical check: top -> bottom
	}else if(c_old == c_new && r_old > r_new){
		for(uint_fast8_t i = r_old - 1; i > r_new; i--){
			if(board_get_piece(b, i, c_old)){
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
			}
		}
	//horizontal check: left -> right
	}else if(r_old == r_new && c_old < c_new){
		for(uint_fast8_t i = c_old + 1; i < c_new; i++){
			if(board_get_piece(b, r_old, i)){
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
			}
		}
	//horizontal check: right -> left
	}else if(r_old == r_new && c_old > c_new){
		for(uint_fast8_t i = c_old - 1; i > c_new; i--){
			if(board_get_piece(b, r_old, i)){
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
			}
		}
	//illegal move
	}else{
		if(check_queen){
			*in_range = false;
			return ;
		}
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

static void check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision)
{
	const char *FUNC_NAME = "static void check_move_bishop(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new, bool check_queen, bool *in_range, bool *collision)";

	if(check_queen){
		*in_range = true;
		*collision = false;
	}

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
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
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
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
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
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
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
				if(check_queen){
					*collision = true;
					return ;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return ;
			}
		}
	//illegal move
	}else{
		if(check_queen){
			*in_range = false;
			return ;
		}
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

static void check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "static void check_move_queen(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

	bool bishop_in_range;
	bool bishop_collision;
	bool rook_in_range;
	bool rook_collision;

	check_move_bishop(b, r_old, c_old, r_new, c_new, true,
		&bishop_in_range, &bishop_collision);
	check_move_rook(b, r_old, c_old, r_new, c_new, true,
		&rook_in_range, &rook_collision);
	if(!(bishop_in_range || rook_in_range)){
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
	if(bishop_collision || rook_collision){
		call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

static void check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "static void check_move_knight(\
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

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
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

static void check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "static void check_move_pawn(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

	//black forward
	if(
		r_new - r_old == 1
		&&
		c_new - c_old == 0
		&&
		piece_get_color(board_get_piece(b, r_old, c_old)) == BLACK
	){
		if(board_get_piece(b, r_new, c_new)){
			call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
				FUNC_NAME);
			return ;
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
			call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
				FUNC_NAME);
			return ;
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
			call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
				FUNC_NAME);
			return ;
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
			call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
				FUNC_NAME);
			return ;
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
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return ;
	}
}

ErrFncPtr gl_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)
{
	const char *FUNC_NAME = "ErrFncPtr gl_set_err_hndl(\
enum ErrorCode error_type, ErrFncPtr err_hndl)";

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
		||
		error_type != INVALID_ENUM_PARAM
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
