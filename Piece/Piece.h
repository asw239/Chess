#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

enum PieceType{
	king,
	queen,
	rook,
	bishop,
	knight,
	pawn
};

typedef struct PieceStruct *Piece;

Piece piece_create(void);

void piece_destroy(Piece p);

void piece_set_type(Piece p, enum PieceType pt);

enum PieceType piece_get_type(Piece p);

void piece_set_pos(Piece p, uint_fast8_t x, uint_fast8_t y);

uint_fast8_t piece_get_x(Piece p);

uint_fast8_t piece_get_y(Piece p);

void piece_set_color(Piece p, uint_fast8_t c);

uint_fast8_t piece_get_color(Piece p);

#endif
