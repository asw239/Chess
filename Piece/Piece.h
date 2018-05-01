#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

#define PIECE_COUNT 16

enum PieceType{
	KING = 1,
	QUEEN = 2,
	ROOK = 3,
	BISHOP = 4,
	KNIGHT = 5,
	PAWN = 6
};

enum PieceColor{
	BLACK,
	WHITE
};

typedef struct PieceStruct *Piece;

Piece piece_create(void);

Piece piece_create_copy(const Piece p);

void piece_destroy(Piece *p);

void piece_set_type(Piece p, enum PieceType pt);

enum PieceType piece_get_type(const Piece p);

void piece_set_pos(Piece p, uint_fast8_t r, uint_fast8_t c);

uint_fast8_t piece_get_r(const Piece p);

uint_fast8_t piece_get_c(const Piece p);

void piece_set_color(Piece p, enum PieceColor c);

enum PieceColor piece_get_color(const Piece p);

ErrFncPtr piece_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl);

#endif
