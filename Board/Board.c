#include "Board.h"
#include "../game_logic/game_logic.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>
#include <stdio.h>

static const char *FILE_NAME = "Board.c";
const uint_fast8_t BOARD_SIZE = 8;
static ErrFncPtr err_fnc_arr[ERROR_CODE_COUNT] = {[GLOBAL_ERROR] = def_hndl};

struct BoardStruct{
	Piece **board_arr;
	enum PieceColor *turn;
	Piece *w_capture_list;
	Piece *b_capture_list;
	bool *unmoved_pieces;
	Piece en_passant_pawn;
};

Board board_create(void)
{
	const char *FUNC_NAME = "Board board_create(void)";

	Board b;
	b = malloc(sizeof(*b));
	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	b->board_arr = malloc(sizeof(Piece*) * BOARD_SIZE);
	if(!b->board_arr){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++){
		b->board_arr[i] = calloc(BOARD_SIZE, sizeof(Piece));
		if(!b->board_arr[i]){
			call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
			return NULL;
		}
	}

	b->turn = malloc(sizeof(enum PieceColor));
	if(!b->turn){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	b->unmoved_pieces = malloc(sizeof(bool) * UNMOVED_PIECES_COUNT);
	if(!b->unmoved_pieces){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	b->b_capture_list = NULL;
	b->w_capture_list = NULL;
	b->en_passant_pawn = NULL;

	return b;
}

Board board_create_copy(const Board b)
{
	const char *FUNC_NAME = "Board board_create_copy(const Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	Board new_b = board_create();

	for(uint_fast8_t i = 0; i < BOARD_SIZE; i++)
		for(uint_fast8_t j = 0; j < BOARD_SIZE; j++)
			if(b->board_arr[i][j])
				new_b->board_arr[i][j] =
					piece_create_copy(b->board_arr[i][j]);
	*new_b->turn = *b->turn;
	if(b->b_capture_list && b->w_capture_list){
		board_init_capture_list(new_b);
		for(uint_fast8_t i = 0; i < PIECE_COUNT; i++){
			if(b->b_capture_list[i])
				new_b->b_capture_list[i] =
					piece_create_copy(b->b_capture_list[i]);
			if(b->w_capture_list[i])
				new_b->w_capture_list[i] =
					piece_create_copy(b->w_capture_list[i]);
		}
	}
	for(uint_fast8_t i = 0; i < UNMOVED_PIECES_COUNT; i++)
		new_b->unmoved_pieces[i] = b->unmoved_pieces[i];

	if(b->en_passant_pawn){
		uint_fast8_t epp_r = piece_get_r(b->en_passant_pawn);
		uint_fast8_t epp_c = piece_get_c(b->en_passant_pawn);
		new_b->en_passant_pawn= new_b->board_arr[epp_r][epp_c];
	}

	return new_b;
}

void board_destroy(Board *b)
{
	const char *FUNC_NAME = "void board_destroy(Board *b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

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
	const char *FUNC_NAME = "void board_link_piece(Board b, Piece p, \
uint_fast8_t r, uint_fast8_t c)";

	if(!b || !p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	if(b->board_arr[r][c]){
		call_error(err_fnc_arr, BOARD_NONEMPTY_SQUARE, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	b->board_arr[r][c] = p;
	piece_set_pos(p, r, c);
}

void board_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new)
{
	const char *FUNC_NAME = "void board_move_piece(Board b, \
uint_fast8_t r_old, uint_fast8_t c_old, uint_fas8t_t r_new, \
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

	if(!b->board_arr[r_old][c_old]){
		call_error(err_fnc_arr, BOARD_EMPTY_SQUARE, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	if(piece_get_type(b->board_arr[r_old][c_old]) == KING){
		if(check_castle(b, r_old, c_old, c_new)){
			piece_set_pos(b->board_arr[r_old][c_old], r_new, c_new);
			b->board_arr[r_new][c_new] = b->board_arr[r_old][c_old];
			b->board_arr[r_old][c_old] = NULL;

			enum PieceColor king_c =
				piece_get_color(b->board_arr[r_new][c_new]);
			if(c_old - 2 == c_new && king_c == WHITE){
				piece_set_pos(b->board_arr[7][0], 7, 2);
				b->board_arr[7][2] = b->board_arr[7][0];
				b->board_arr[7][0] = NULL;

			}else if(c_old + 2 == c_new && king_c == WHITE){
				piece_set_pos(b->board_arr[7][7], 7, 4);
				b->board_arr[7][4] = b->board_arr[7][7];
				b->board_arr[7][7] = NULL;

			}else if(c_old - 2 == c_new && king_c == BLACK){
				piece_set_pos(b->board_arr[0][0], 0, 2);
				b->board_arr[0][2] = b->board_arr[0][0];
				b->board_arr[0][0] = NULL;

			}else if(c_old + 2 == c_new && king_c == BLACK){
				piece_set_pos(b->board_arr[0][7], 0, 4);
				b->board_arr[0][4] = b->board_arr[0][7];
				b->board_arr[0][7] = NULL;
			}

			return ;
		}
	}

	if(piece_get_type(b->board_arr[r_old][c_old]) == PAWN){
		if(check_en_passant(b, r_old, c_old, r_new, c_new)){
			piece_set_pos(b->board_arr[r_old][c_old], r_new, c_new);
			b->board_arr[r_new][c_new] = b->board_arr[r_old][c_old];
			b->board_arr[r_old][c_old] = NULL;

			if(b->b_capture_list && b->w_capture_list)
				board_remove_piece(b, r_old, c_new, false);
			else
				board_remove_piece(b, r_old, c_new, true);

			return ;
		}
	}

	if(!validate_move(b, r_old, c_old, r_new, c_new))
		return ;

	if(b->board_arr[r_new][c_new]){
		if(b->b_capture_list && b->w_capture_list)
			board_remove_piece(b, r_new, c_new, false);
		else
			board_remove_piece(b, r_new, c_new, true);
	}

	piece_set_pos(b->board_arr[r_old][c_old], r_new, c_new);
	b->board_arr[r_new][c_new] = b->board_arr[r_old][c_old];
	b->board_arr[r_old][c_old] = NULL;

	if(r_old == 7 && c_old == 0 && !board_has_piece_moved(b, W_L_ROOK))
		board_set_piece_moved(b, W_L_ROOK, true);
	if(r_old == 7 && c_old == 7 && !board_has_piece_moved(b, W_R_ROOK))
		board_set_piece_moved(b, W_R_ROOK, true);
	if(r_old == 7 && c_old == 3 && !board_has_piece_moved(b, W_KING))
		board_set_piece_moved(b, W_KING, true);
	if(r_old == 0 && c_old == 0 && !board_has_piece_moved(b, B_L_ROOK))
		board_set_piece_moved(b, B_L_ROOK, true);
	if(r_old == 0 && c_old == 7 && !board_has_piece_moved(b, B_R_ROOK))
		board_set_piece_moved(b, B_R_ROOK, true);
	if(r_old == 0 && c_old == 3 && !board_has_piece_moved(b, B_KING))
		board_set_piece_moved(b, B_KING, true);

	if(
		piece_get_type(b->board_arr[r_new][c_new]) == PAWN
		&&
		(r_new - r_old == 2 || r_new - r_old == -2)
	)
		board_set_en_passant_pawn(b, b->board_arr[r_new][c_new]);
	else
		board_set_en_passant_pawn(b, NULL);
}

Piece board_get_piece(Board b, uint_fast8_t r, uint_fast8_t c)
{
	const char *FUNC_NAME = "Piece board_get_piece(Board b, \
uint_fast8_t r, uint_fast8_t c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	return b->board_arr[r][c];
}

void board_set_turn(Board b, enum PieceColor c)
{
	const char *FUNC_NAME = "void board_set_turn(Board b, \
enum PieceColor c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(c != BLACK && c != WHITE){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	*(b->turn) = c;
}

void board_flip_turn(Board b)
{
	const char *FUNC_NAME = "void board_flip_turn(Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	*(b->turn) == WHITE ? (*(b->turn) = BLACK) : (*(b->turn) = WHITE);
}

enum PieceColor board_get_turn(const Board b)
{
	const char *FUNC_NAME = "enum PieceColor board_get_turn(const Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return 0;
	}

	return *(b->turn);
}

Piece board_remove_piece(Board b, uint_fast8_t r, uint_fast8_t c, bool destroy)
{
	const char *FUNC_NAME = "Piece board_remove_piece(Board b, \
uint_fast8_t r, uint_fast8_t c, bool destroy)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	if(r >= BOARD_SIZE || c >= BOARD_SIZE){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	if(!b->board_arr[r][c]){
		call_error(err_fnc_arr, BOARD_EMPTY_SQUARE, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	if(destroy){
		if(b->b_capture_list || b->w_capture_list)
			call_error(err_fnc_arr, CONFLICTING_PARAM, FILE_NAME,
				FUNC_NAME);

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
}

void board_init_capture_list(Board b)
{
	const char *FUNC_NAME = "void board_init_capture_list(Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	b->b_capture_list = calloc(PIECE_COUNT, sizeof(Piece));
	if(!b->b_capture_list){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return ;
	}

	b->w_capture_list = calloc(PIECE_COUNT, sizeof(Piece));
	if(!b->w_capture_list){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return ;
	}
}

Piece *board_get_capture_list(const Board b, enum PieceColor c)
{
	const char *FUNC_NAME = "Piece *board_get_capture_list(const Board b, \
enum PieceColor c)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	if(c != WHITE && c != BLACK){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	return c == WHITE ? b->w_capture_list : b->b_capture_list;
}

void board_set_piece_moved(Board b, enum UnmovedPieces p, bool moved)
{
	const char *FUNC_NAME = "void board_set_piece_moved(Board b, \
enum UnmovedPieces p, bool moved)";
	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(p != W_L_ROOK && p != W_R_ROOK && p != W_KING &&
		p != B_L_ROOK && p != B_R_ROOK && p != B_KING){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FUNC_NAME,
			FILE_NAME);
		return ;
	}

	b->unmoved_pieces[p] = moved;
}

bool board_has_piece_moved(const Board b, enum UnmovedPieces p)
{
	const char *FUNC_NAME = "bool board_has_piece_moved(const Board b, \
enum UnmovedPieces p)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return false;
	}

	if(p != W_L_ROOK && p != W_R_ROOK && p != W_KING &&
		p != B_L_ROOK && p != B_R_ROOK && p != B_KING){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return false;
	}

	return b->unmoved_pieces[p];
}

Piece **board_get_board_arr(const Board b)
{
	const char *FUNC_NAME = "Piece **board_get_board_arr(const Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	return b->board_arr;
}

void board_set_en_passant_pawn(Board b, const Piece p)
{
	const char *FUNC_NAME = "void board_set_en_passant_pawn(Board b, \
const Piece p)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(p && piece_get_type(p) != PAWN){
		call_error(err_fnc_arr, BOARD_INVALID_EN_PASSANT_PIECE,
			FILE_NAME, FUNC_NAME);
		return ;
	}

	b->en_passant_pawn = p;
}

Piece board_get_en_passant_pawn(const Board b)
{
	const char *FUNC_NAME = "void board_set_en_passant_pawn(Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	return b->en_passant_pawn;
}

ErrFncPtr board_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)
{
	const char *FUNC_NAME = "ErrFncPtr board_set_err_hndl("\
"enum ErrorCode error_type, ErrFncPtr err_hndl)";

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
		||
		error_type != PIECE_MOVE_KING_CHECKED
		||
		BOARD_INVALID_EN_PASSANT_PIECE
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
