#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "../Piece/Piece.h"
#include "../errors/ErrorCode.h"

enum UnmovedPieces{
	W_L_ROOK,
	W_R_ROOK,
	W_KING,
	B_L_ROOK,
	B_R_ROOK,
	B_KING,
	UNMOVED_PIECES_COUNT
};

typedef struct BoardStruct *Board;

Board board_create(void);

void board_destroy(Board *b);

void board_link_piece(Board b, Piece p, uint_fast8_t r, uint_fast8_t c);

void board_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);

Piece board_get_piece(Board b, uint_fast8_t r, uint_fast8_t c);

uint_fast8_t board_get_size(void);

void board_set_turn(Board b, enum PieceColor c);

void board_flip_turn(Board b);

enum PieceColor board_get_turn(const Board b);

Piece board_remove_piece(Board b, uint_fast8_t r, uint_fast8_t c, bool destroy);

void board_init_capture_list(Board b);

Piece *board_get_capture_list(const Board b, enum PieceColor c);

void board_set_piece_moved(Board b, enum UnmovedPieces p, bool moved);

bool board_has_piece_moved(const Board b, enum UnmovedPieces p);

void board_set_king_checked(Board b, enum PieceColor c, bool checked);

bool board_is_king_checked(const Board b, enum PieceColor c);

void board_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
