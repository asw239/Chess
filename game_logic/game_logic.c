#include "game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>

static bool check_move_king(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);
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
static bool check_castle_attempt(uint_fast8_t c_old, uint_fast8_t c_new,
	bool *attempting_left_castle, bool *attempting_right_castle);
static bool check_castle_king_moved(Board b, enum PieceColor king_c);
static bool check_castle_rook_moved(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle);
static bool check_castle_pieces_inbetween(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle);
static bool check_castle_king_in_check(const Board b, enum PieceColor king_c);
static bool check_castle_through_check(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle);

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

bool validate_move(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	static const char *FUNC_NAME = "bool validate_move(const Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

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

	if(!board_get_piece(b, r_old, c_old)){
		call_error(err_fnc_arr, BOARD_EMPTY_SQUARE, FILE_NAME,
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
		return check_move_king(r_old, c_old, r_new, c_new);
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

static bool check_move_king(uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "static bool check_move_king(\
uint_fast8_t r_old, uint_fast8_t c_old, uint_fast8_t r_new, \
uint_fast8_t c_new)";

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
	const char *FUNC_NAME = "static bool check_move_rook(const Board b, \
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
					return false;
				}
				call_error(err_fnc_arr, PIECE_MOVE_COLLISION,
					FILE_NAME, FUNC_NAME);
				return false;
			}
		}
	//vertical check: top -> bottom
	}else if(c_old == c_new && r_old > r_new){
		for(uint_fast8_t i = r_old - 1; i > r_new; i--){
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
		for(uint_fast8_t i = c_old + 1; i < c_new; i++){
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
	const char *FUNC_NAME = "static bool check_move_bishop(const Board b, \
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
			i--, j++){
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
			i++, j--){
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
	const char *FUNC_NAME = "static bool check_move_queen(const Board b, \
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
	const char *FUNC_NAME = "static bool check_move_knight(\
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
		return false;
	}
	return true;
}

static bool check_move_pawn(const Board b, uint_fast8_t r_old,
	uint_fast8_t c_old, uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "static bool check_move_pawn(const Board b, \
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

bool check(const Board b, enum PieceColor c)
{
	const char *FUNC_NAME = "bool check(const Board b, enum PieceColor c)";

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

	bool return_val = false;
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++){
			Piece curr_p = board_get_board_arr(b)[i][j];
			if(
				curr_p
				&&
				piece_get_color(curr_p) != c
				&&
				!(i == king_r && j == king_c)
				&&
				validate_move(b, i, j, king_r, king_c)
			){
				return_val = true;
				goto LOOPS_EXIT;
			}
		}
	}
	LOOPS_EXIT:

	gl_set_err_hndl(PIECE_MOVE_COLLISION, collision_cback);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, not_in_range_cback);

	return return_val;
}

static Piece find_king(const Board b, enum PieceColor c)
{
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++){
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

bool mate(const Board b, enum PieceColor c)
{
	const char *FUNC_NAME = "bool mate(const Board b, enum PieceColor c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return false;
	}

	if(c != WHITE && c != BLACK){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	if(!check(b, c))
		return false;

	Piece *active_allies_list = generate_active_allies_list(b, c);
	bool return_val = true;
	for(uint_fast8_t i = 0; active_allies_list[i]; i++)
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

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++){
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

	bool return_val = false;
	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++){
			if(
				!(piece_r == i && piece_c == j)
				&&
				validate_move(b, piece_r, piece_c, i, j)
			){
				Board b_cpy = board_create_copy(b);
				board_move_piece(b_cpy, piece_r, piece_c, i, j);
				if(!check(b_cpy, c)){
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

	return return_val;
}

bool check_castle(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t c_new)
{
	enum PieceColor king_c =
		piece_get_color(board_get_board_arr(b)[r_old][c_old]);
	bool attempting_left_castle;
	bool attempting_right_castle;

	if(!check_castle_attempt(c_old, c_new, &attempting_left_castle,
		&attempting_right_castle))
		return false;

	if(check_castle_king_moved(b, king_c))
		return false;

	if(check_castle_rook_moved(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return false;

	if(check_castle_pieces_inbetween(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return false;

	if(check_castle_king_in_check(b, king_c))
		return false;

	if(check_castle_through_check(b, king_c, attempting_left_castle,
		attempting_right_castle))
		return false;

	return true;
}

static bool check_castle_attempt(uint_fast8_t c_old, uint_fast8_t c_new,
	bool *attempting_left_castle, bool *attempting_right_castle)
{
	*attempting_left_castle = false;
	*attempting_right_castle = false;
	if(c_old - 2 == c_new){
		*attempting_left_castle = true;
		return true;

	}else if(c_old + 2 == c_new){
		*attempting_right_castle = true;
		return true;

	}else{
		return false;
	}
}

static bool check_castle_king_moved(const Board b, enum PieceColor king_c)
{
	const char *FUNC_NAME = "static bool check_castle_king_moved(\
const Board b, enum PieceColor king_c)";

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
	const char *FUNC_NAME = "static bool check_castle_rook_moved(\
const Board b, enum PieceColor king_c, bool attempting_left_castle, \
bool attempting_right_castle)";

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

static bool check_castle_pieces_inbetween(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle)
{
	const char *FUNC_NAME = "static bool check_castle_pieces_inbetween(\
const Board b, enum PieceColor king_c, bool attempting_left_castle, \
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

static bool check_castle_king_in_check(const Board b, enum PieceColor king_c)
{
	const char *FUNC_NAME = "static bool check_castle_king_in_check(\
const Board b, enum PieceColor king_c)";

	if(check(b, king_c)){
		call_error(err_fnc_arr, PIECE_MOVE_ILLEGAL_CASTLE, FILE_NAME,
			FUNC_NAME);
		return true;

	}else{
		return false;
	}
}

static bool check_castle_through_check(const Board b, enum PieceColor king_c,
	bool attempting_left_castle, bool attempting_right_castle)
{
	const char *FUNC_NAME = "static bool check_castle_through_check(\
const Board b, enum PieceColor king_c, bool attempting_left_castle, \
bool attempting_right_castle)";

	if(
		king_c == WHITE
		&&
		attempting_left_castle
	){
		Board b_cpy = board_create_copy(b);
		board_move_piece(b_cpy, 7, 3, 7, 2);
		if(check(b_cpy, king_c)){
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
		board_move_piece(b_cpy, 7, 3, 7, 4);
		if(check(b_cpy, king_c)){
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
		board_move_piece(b_cpy, 0, 3, 0, 2);
		if(check(b_cpy, king_c)){
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
		board_move_piece(b_cpy, 0, 3, 0, 4);
		if(check(b_cpy, king_c)){
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
		||
		error_type != PIECE_MOVE_ILLEGAL_CASTLE
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
