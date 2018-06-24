#include <stdlib.h>
#include "game_logic.h"
#include "../errors/def_hndl.h"

static void general_move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void castle_move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static void en_passant_move_logic(Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static void move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_move_king(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision);
static bool check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision);
static bool check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static Piece find_king(const Board b, enum PieceColor c);
static void null_func(enum ErrorCode err, const char *msg);
static Piece *generate_active_allies_list(const Board b, enum PieceColor c);
static bool can_defend_mate(Board b, Piece p, enum PieceColor c);
static enum CheckReturn check_castle(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_castle_attempt(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t c_new, uint_fast8_t r_new,
	bool *attempting_left_castle, bool *attempting_right_castle);
static bool check_castle_king_moved(Board b, enum PieceColor king_c);
static bool check_castle_rook_moved(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle);
static bool check_castle_pieces_inbetween(const Board b,
	enum PieceColor king_c, bool attempting_left_castle,
	bool attempting_right_castle);
static bool check_castle_king_in_check(const Board b,
	enum PieceColor king_c);
static bool check_castle_through_check(const Board b,
	enum PieceColor king_c, bool attempting_left_castle,
	bool attempting_right_castle);
static enum CheckReturn check_en_passant(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_en_passant_attempt(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new);
static bool check_en_passant_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_new);

static ErrFncPtr err_fnc_arr[ERROR_CODE_COUNT] = {[GLOBAL_ERROR] =
	errors_def_hndl};
static const char *FILE_NAME = "game_logic.c";

enum CheckReturn{
	SUCCESS,
	FAIL,
	NO_ATTEMPT
};

Board gl_generate_start_board(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);
	board_init_capture_list(b);

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
		for(int j = 0; j < BOARD_SIZE; ++j){
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

bool gl_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"bool gl_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old, \
uint_fast8_t r_new, uint_fast8_t c_new)";

	if(
		board_get_turn(b)
		!=
		piece_get_color(board_get_board_arr(b)[r_old][c_old])
	  ){
		call_error(err_fnc_arr, PIECE_MOVE_UNEXPECTED_COLOR, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	if(!gl_validate_move(b, r_old, c_old, r_new, c_new))
		return false;

	if(SUCCESS == check_castle(b, r_old, c_old, r_new, c_new)){
		castle_move_logic(b, r_old, c_old, r_new, c_new);
		return true;
	}

	if(SUCCESS == check_en_passant(b, r_old, c_old, r_new, c_new)){
		en_passant_move_logic(b, r_old, c_old, r_new, c_new);
		return true;
	}

	general_move_logic(b, r_old, c_old, r_new, c_new);
	return true;
}

bool gl_validate_move(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"bool gl_validate_move(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return false;
	}

	if(r_old >= BOARD_SIZE || c_old >= BOARD_SIZE ||
		r_new >= BOARD_SIZE || c_new >= BOARD_SIZE){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	if(r_old == r_new && c_old == c_new){
		call_error(err_fnc_arr, PIECE_MOVE_SAME_POS, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	if(
		board_get_piece(b, r_new, c_new)
		&&
		piece_get_color(board_get_piece(b, r_new, c_new))
		==
		piece_get_color(board_get_piece(b, r_old, c_old))
	){
		call_error(err_fnc_arr, PIECE_MOVE_OVERLAPS_ALLY, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	switch(piece_get_type(board_get_piece(b, r_old, c_old))){
	case KING:
		return check_move_king(b, r_old, c_old, r_new, c_new);
	case ROOK:
		return check_move_rook(b, r_old, c_old, r_new, c_new, false,
			NULL, NULL);
	case BISHOP:
		return check_move_bishop(b, r_old, c_old, r_new, c_new, false,
			NULL, NULL);
	case QUEEN:
		return check_move_queen(b, r_old, c_old, r_new, c_new);
	case KNIGHT:
		return check_move_knight(r_old, c_old, r_new, c_new);
	case PAWN:
		return check_move_pawn(b, r_old, c_old, r_new, c_new);
	}

	return false;
}

static void general_move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	Piece **arr = board_get_board_arr(b);

	if(arr[r_new][c_new])
		board_remove_piece(b, r_new, c_new,
			board_get_capture_list(b, BLACK)? false : true);

	move_logic(b, r_old, c_old, r_new, c_new);

	if(r_old == 7 && c_old == 0 && !board_has_piece_moved(b, W_L_ROOK))
		board_set_piece_moved(b, W_L_ROOK, true);

	else if(r_old == 7 && c_old == 7 && !board_has_piece_moved(b, W_R_ROOK))
		board_set_piece_moved(b, W_R_ROOK, true);

	else if(r_old == 7 && c_old == 3 && !board_has_piece_moved(b, W_KING))
		board_set_piece_moved(b, W_KING, true);

	else if(r_old == 0 && c_old == 0 && !board_has_piece_moved(b, B_L_ROOK))
		board_set_piece_moved(b, B_L_ROOK, true);

	else if(r_old == 0 && c_old == 7 && !board_has_piece_moved(b, B_R_ROOK))
		board_set_piece_moved(b, B_R_ROOK, true);

	else if(r_old == 0 && c_old == 3 && !board_has_piece_moved(b, B_KING))
		board_set_piece_moved(b, B_KING, true);

	if(
		piece_get_type(arr[r_new][c_new]) == PAWN
		&&
		(r_new - r_old == 2 || r_new - r_old == -2)
	)
		(board_set_en_passant_pawn(b, arr[r_new][c_new]));
	else
		board_set_en_passant_pawn(b, NULL);
}

static void castle_move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	Piece **arr = board_get_board_arr(b);

	move_logic(b, r_old, c_old, r_new, c_new);

	enum PieceColor king_c = piece_get_color(arr[r_new][c_new]);
	if(c_old - 2 == c_new && king_c == WHITE){
		piece_set_pos(arr[7][0], 7, 2);
		arr[7][2] = arr[7][0];
		arr[7][0] = NULL;

	}else if(c_old + 2 == c_new && king_c == WHITE){
		piece_set_pos(arr[7][7], 7, 4);
		arr[7][4] = arr[7][7];
		arr[7][7] = NULL;

	}else if(c_old - 2 == c_new && king_c == BLACK){
		piece_set_pos(arr[0][0], 0, 2);
		arr[0][2] = arr[0][0];
		arr[0][0] = NULL;

	}else if(c_old + 2 == c_new && king_c == BLACK){
		piece_set_pos(arr[0][7], 0, 4);
		arr[0][4] = arr[0][7];
		arr[0][7] = NULL;
	}
}

static void en_passant_move_logic(Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	board_remove_piece(b, r_old, c_new,
		board_get_capture_list(b, BLACK)? false : true);

	move_logic(b, r_old, c_old, r_new, c_new);
}

static void move_logic(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	Piece **arr = board_get_board_arr(b);

	piece_set_pos(arr[r_old][c_old], r_new, c_new);
	arr[r_new][c_new] = arr[r_old][c_old];
	arr[r_old][c_old] = NULL;
}

static bool check_move_king(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"static bool check_move_king(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)";

	enum CheckReturn cr_castle =
		check_castle(b, r_old, c_old, r_new, c_new);
	if(cr_castle == FAIL)
		return false;
	else if(cr_castle == SUCCESS)
		return true;

	if(
		r_old - 1 > r_new || r_old + 1 < r_new
		||
		c_old - 1 > c_new || c_old + 1 < c_new
	){
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	return true;
}

static bool check_move_rook(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision)
{
	static const char *FUNC_NAME =
"static bool check_move_rook(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new, \
bool check_queen, bool *in_range, bool *collision)";

	if(check_queen){
		*in_range = true;
		*collision = false;
	}

	//vertical check: bottom -> top
	if(c_old == c_new && r_old < r_new){
		for(uint_fast8_t i = r_old + 1; i < r_new; ++i){
			if(board_get_piece(b, i, c_old)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//vertical check: top -> bottom
	}else if(c_old == c_new && r_old > r_new){
		for(uint_fast8_t i = r_old - 1; i > r_new; --i){
			if(board_get_piece(b, i, c_old)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//horizontal check: left -> right
	}else if(r_old == r_new && c_old < c_new){
		for(uint_fast8_t i = c_old + 1; i < c_new; ++i){
			if(board_get_piece(b, r_old, i)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//horizontal check: right -> left
	}else if(r_old == r_new && c_old > c_new){
		for(uint_fast8_t i = c_old - 1; i > c_new; i--){
			if(board_get_piece(b, r_old, i)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//illegal move
	}else{
		if(check_queen){
			*in_range = false;
			return false;
		}
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return false;
	}
	return true;
}

static bool check_move_bishop(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool check_queen, bool *in_range, bool *collision)
{
	static const char *FUNC_NAME =
"static bool check_move_bishop(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new, \
bool check_queen, bool *in_range, bool *collision)";

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
			++i, ++j){
			if(board_get_piece(b, i, j)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
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
			--i, ++j){
			if(board_get_piece(b, i, j)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
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
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
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
			++i, --j){
			if(board_get_piece(b, i, j)){
				if(check_queen){
					*collision = true;
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//illegal move
	}else{
		if(check_queen){
			*in_range = false;
			return false;
		}
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return false;
	}
	return true;
}

static bool check_move_queen(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"static bool check_move_queen(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)";

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
		return false;
	}
	if(bishop_collision || rook_collision){
		call_error(err_fnc_arr, PIECE_MOVE_COLLISION, FILE_NAME,
			FUNC_NAME);
		return false;
	}
	return true;
}

static bool check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"static bool check_move_knight(uint_fast8_t r_old, uint_fast8_t c_old, \
uint_fast8_t r_new, uint_fast8_t c_new)";

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
		return false;
	}
	return true;
}

static bool check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"static bool check_move_pawn(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)";

	enum CheckReturn cr_en_passant =
		check_en_passant(b, r_old, c_old, r_new, c_new);
	if(cr_en_passant == FAIL)
		return false;
	else if(cr_en_passant == SUCCESS)
		return true;

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
			return false;
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
			return false;
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
			return false;
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
			return false;
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
		return true;
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
		return true;
	//illegal move
	}else{
		call_error(err_fnc_arr, PIECE_MOVE_NOT_IN_RANGE, FILE_NAME,
			FUNC_NAME);
		return false;
	}
	return true;
}

bool gl_check(const Board b, enum PieceColor c)
{
	static const char *FUNC_NAME =
"bool gl_check(const Board b, enum PieceColor c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return false;
	}

	if(c != WHITE && c != BLACK){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	Piece king = find_king(b, c);
	uint_fast8_t king_r = piece_get_r(king);
	uint_fast8_t king_c = piece_get_c(king);

	ErrFncPtr collision_cback =
		gl_set_err_hndl(PIECE_MOVE_COLLISION, null_func);
	ErrFncPtr not_in_range_cback =
		gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, null_func);
	ErrFncPtr ally_overlap_cback =
		gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, null_func);
	ErrFncPtr illegal_castle_cback =
		gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, null_func);
	ErrFncPtr illegal_en_passant_cback =
		gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE, null_func);

	bool return_val = false;
	board_flip_turn(b);
	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; ++j){
			Piece curr_p = board_get_board_arr(b)[i][j];
			if(
				curr_p
				&&
				piece_get_color(curr_p) != c
				&&
				!(i == king_r && j == king_c)
				&&
				gl_validate_move(b, i, j, king_r, king_c)
			){
				return_val = true;
				goto LOOPS_EXIT;
			}
		}
	}
	LOOPS_EXIT:
	board_flip_turn(b);

	gl_set_err_hndl(PIECE_MOVE_COLLISION, collision_cback);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, not_in_range_cback);
	gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, ally_overlap_cback);
	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, illegal_castle_cback);
	gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE,
		illegal_en_passant_cback);

	return return_val;
}

static Piece find_king(const Board b, enum PieceColor c)
{
	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; ++j){
			Piece curr_p = board_get_board_arr(b)[i][j];
			if(
				curr_p
				&&
				piece_get_type(curr_p) == KING
				&&
				piece_get_color(curr_p) == c
			)
				return curr_p;
		}
	}
	return NULL;
}

static void null_func(enum ErrorCode err, const char *msg)
{
	;
}

bool gl_mate(const Board b, enum PieceColor c)
{
	static const char *FUNC_NAME =
"bool gl_mate(const Board b, enum PieceColor c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return false;
	}

	if(c != WHITE && c != BLACK){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	if(!gl_check(b, c))
		return false;

	Piece *active_allies_list = generate_active_allies_list(b, c);
	bool return_val = true;
	for(uint_fast8_t i = 0; active_allies_list[i]; ++i)
		if(can_defend_mate(b, active_allies_list[i], c)){
			return_val = false;
			break;
		}

	free(active_allies_list);
	return return_val;
}

static Piece *generate_active_allies_list(const Board b, enum PieceColor c)
{
	Piece *list = calloc(PIECE_COUNT, sizeof(Piece));
	uint_fast8_t list_i = 0;

	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; ++j){
			Piece curr_piece = board_get_board_arr(b)[i][j];
			if(curr_piece && piece_get_color(curr_piece) == c)
				list[list_i++] = curr_piece;
		}
	}

	return list;
}

static bool can_defend_mate(Board b, Piece p, enum PieceColor c)
{
	uint_fast8_t piece_r = piece_get_r(p);
	uint_fast8_t piece_c = piece_get_c(p);

	ErrFncPtr collision_cback =
		gl_set_err_hndl(PIECE_MOVE_COLLISION, null_func);
	ErrFncPtr not_in_range_cback =
		gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, null_func);
	ErrFncPtr ally_overlap_cback =
		gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, null_func);
	ErrFncPtr illegal_castle_cback =
		gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, null_func);
	ErrFncPtr illegal_en_passant_cback =
		gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE, null_func);

	bool return_val = false;
	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; ++j){
			if(
				!(piece_r == i && piece_c == j)
				&&
				gl_validate_move(b, piece_r, piece_c, i, j)
			){
				Board b_cpy = board_create_copy(b);
				gl_move_piece(b_cpy, piece_r, piece_c, i, j);
				if(!gl_check(b_cpy, c)){
					board_destroy(&b_cpy);
					return_val = true;
					goto LOOPS_EXIT;
				}else{
					board_destroy(&b_cpy);
				}
			}
		}
	}
	LOOPS_EXIT:

	gl_set_err_hndl(PIECE_MOVE_COLLISION, collision_cback);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, not_in_range_cback);
	gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, ally_overlap_cback);
	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, illegal_castle_cback);
	gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE,
		illegal_en_passant_cback);

	return return_val;
}

static enum CheckReturn check_castle(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	enum PieceColor king_c =
		piece_get_color(board_get_board_arr(b)[r_old][c_old]);
	bool attempting_left_castle;
	bool attempting_right_castle;

	if(!check_castle_attempt(b, r_old, c_old, r_new, c_new,
		&attempting_left_castle, &attempting_right_castle))
		return NO_ATTEMPT;

	if(check_castle_king_moved(b, king_c))
		return FAIL;

	if(check_castle_rook_moved(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return FAIL;

	if(check_castle_pieces_inbetween(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return FAIL;

	if(check_castle_king_in_check(b, king_c))
		return FAIL;

	if(check_castle_through_check(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return FAIL;

	return SUCCESS;
}

static bool check_castle_attempt(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new,
	bool *attempting_left_castle, bool *attempting_right_castle)
{
	*attempting_left_castle = false;
	*attempting_right_castle = false;
	if(piece_get_type(board_get_board_arr(b)[r_old][c_old]) != KING){
		return false;

	}else if(c_old - 2 == c_new && r_old == r_new){
		*attempting_left_castle = true;
		return true;

	}else if(c_old + 2 == c_new && r_old == r_new){
		*attempting_right_castle = true;
		return true;

	}else{
		return false;
	}
}

static bool check_castle_king_moved(const Board b, enum PieceColor king_c)
{
	static const char *FUNC_NAME =
"static bool check_castle_king_moved(const Board b, enum PieceColor king_c)";

	if(king_c == WHITE && board_has_piece_moved(b, W_KING)){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(king_c == BLACK && board_has_piece_moved(b, B_KING)){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else{
		return false;
	}
}

static bool check_castle_rook_moved(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle)
{
	static const char *FUNC_NAME =
"static bool check_castle_rook_moved(const Board b, enum PieceColor king_c, \
bool attempting_left_castle, bool attempting_right_castle)";

	if(
		king_c == WHITE
		&&
		attempting_left_castle
		&&
		board_has_piece_moved(b, W_L_ROOK)
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == WHITE
		&&
		attempting_right_castle
		&&
		board_has_piece_moved(b, W_R_ROOK)
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == BLACK
		&&
		attempting_left_castle
		&&
		board_has_piece_moved(b, B_L_ROOK)
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == BLACK
		&&
		attempting_right_castle
		&&
		board_has_piece_moved(b, B_R_ROOK)
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else{
		return false;
	}
}

static bool check_castle_pieces_inbetween(const Board b,
	enum PieceColor king_c, bool attempting_left_castle,
	bool attempting_right_castle)
{
	static const char *FUNC_NAME =
"static bool check_castle_pieces_inbetween(const Board b, \
enum PieceColor king_c, bool attempting_left_castle, \
bool attempting_right_castle)";

	Piece **board_arr = board_get_board_arr(b);

	if(
		king_c == WHITE
		&&
		attempting_left_castle
		&&
		(board_arr[7][1] || board_arr[7][2])
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == WHITE
		&&
		attempting_right_castle
		&&
		(board_arr[7][4] || board_arr[7][5] || board_arr[7][6])
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == BLACK
		&&
		attempting_left_castle
		&&
		(board_arr[0][1] || board_arr[0][2])
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else if(
		king_c == BLACK
		&&
		attempting_right_castle
		&&
		(board_arr[0][4] || board_arr[0][5] || board_arr[0][6])
	){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else{
		return false;
	}
}

static bool check_castle_king_in_check(const Board b,
	enum PieceColor king_c)
{
	static const char *FUNC_NAME =
"static bool check_castle_king_in_check(const Board b, \
enum PieceColor king_c)";

	if(gl_check(b, king_c)){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else{
		return false;
	}
}

static bool check_castle_through_check(const Board b,
	enum PieceColor king_c, bool attempting_left_castle,
	bool attempting_right_castle)
{
	static const char *FUNC_NAME =
"static bool check_castle_through_check(const Board b, \
enum PieceColor king_c, bool attempting_left_castle, \
bool attempting_right_castle)";

	if(
		king_c == WHITE
		&&
		attempting_left_castle
	){
		Board b_cpy = board_create_copy(b);
		gl_move_piece(b_cpy, 7, 3, 7, 2);
		if(gl_check(b_cpy, king_c)){
			board_destroy(&b_cpy);
			call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE,
				FILE_NAME, FUNC_NAME);
			return true;
		}else{
			board_destroy(&b_cpy);
			return false;
		}

	}else if(
		king_c == WHITE
		&&
		attempting_right_castle
	){
		Board b_cpy = board_create_copy(b);
		gl_move_piece(b_cpy, 7, 3, 7, 4);
		if(gl_check(b_cpy, king_c)){
			board_destroy(&b_cpy);
			call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE,
				FILE_NAME, FUNC_NAME);
			return true;
		}else{
			board_destroy(&b_cpy);
			return false;
		}

	}else if(
		king_c == BLACK
		&&
		attempting_left_castle
	){
		Board b_cpy = board_create_copy(b);
		gl_move_piece(b_cpy, 0, 3, 0, 2);
		if(gl_check(b_cpy, king_c)){
			board_destroy(&b_cpy);
			call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE,
				FILE_NAME, FUNC_NAME);
			return true;
		}else{
			board_destroy(&b_cpy);
			return false;
		}

	}else if(
		king_c == BLACK
		&&
		attempting_right_castle
	){
		Board b_cpy = board_create_copy(b);
		gl_move_piece(b_cpy, 0, 3, 0, 4);
		if(gl_check(b_cpy, king_c)){
			board_destroy(&b_cpy);
			call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE,
				FILE_NAME, FUNC_NAME);
			return true;
		}else{
			board_destroy(&b_cpy);
			return false;
		}
	}

	//should never reach here, exists to satisify -Wreturn-type
	return true;
}

static enum CheckReturn check_en_passant(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	if(!check_en_passant_attempt(b, r_old, c_old, r_new, c_new))
		return NO_ATTEMPT;

	if(!check_en_passant_pawn(b, r_old, c_new))
		return FAIL;

	return SUCCESS;
}

static bool check_en_passant_attempt(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	if(
		piece_get_type(board_get_board_arr(b)[r_old][c_old]) == PAWN
		&&
		piece_get_color(board_get_board_arr(b)[r_old][c_old]) == WHITE
		&&
		r_old - r_new == 1
		&&
		(c_old - c_new == 1 || c_new - c_old == 1)
		&&
		!board_get_board_arr(b)[r_new][c_new]
	)
		return true;

	else if(
		piece_get_type(board_get_board_arr(b)[r_old][c_old]) == PAWN
		&&
		piece_get_color(board_get_board_arr(b)[r_old][c_old]) == BLACK
		&&
		r_new - r_old == 1
		&&
		(c_old - c_new == 1 || c_new - c_old == 1)
		&&
		!board_get_board_arr(b)[r_new][c_new]
	){
		return true;

	}else
		return false;
}

static bool check_en_passant_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_new)
{
	static const char *FUNC_NAME =
"static bool check_en_passant_pawn(const Board b, uint_fast8_t r_old, \
uint_fast8_t c_new)";

	if(
		board_get_en_passant_pawn(b) != NULL
		&&
		board_get_board_arr(b)[r_old][c_new]
		==
		board_get_en_passant_pawn(b)
	){
		return true;

	}else{
		call_error(err_fnc_arr, BOARD_INVALID_EN_PASSANT_PIECE,
			FILE_NAME, FUNC_NAME);
		return false;
	}
}

ErrFncPtr gl_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)
{
	static const char *FUNC_NAME =
"ErrFncPtr gl_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)";

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
		error_type != PIECE_MOVE_UNEXPECTED_COLOR
		||
		error_type != PIECE_MOVE_OVERLAPS_ALLY
		||
		error_type != PIECE_MOVE_NOT_IN_RANGE
		||
		error_type != PIECE_MOVE_COLLISION
		||
		error_type != INVALID_ENUM_PARAM
		||
		error_type != PIECE_MOVE_ILLEGAL_CASTLE
		||
		error_type != BOARD_INVALID_EN_PASSANT_PIECE
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
