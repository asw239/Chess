#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>
#include "../errors/ErrorCode.h"

#define PIECE_COUNT 16

enum PieceType{
	king = 1,
	queen = 2,
	rook = 3,
	bishop = 4,
	knight = 5,
	pawn = 6
};

enum PieceColor{
	BLACK,
	WHITE
};

typedef struct PieceStruct *Piece;

Piece piece_create(void);

void piece_destroy(Piece *p);

void piece_set_type(Piece p, enum PieceType pt);

enum PieceType piece_get_type(const Piece p);

void piece_set_pos(Piece p, uint_fast8_t x, uint_fast8_t y);

uint_fast8_t piece_get_x(const Piece p);

uint_fast8_t piece_get_y(const Piece p);

void piece_set_color(Piece p, enum PieceColor c);

enum PieceColor piece_get_color(const Piece p);

bool piece_set_err_hndl(void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
