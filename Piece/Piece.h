#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>
#include <stdbool.h>

enum PieceType{
	king,
	queen,
	rook,
	bishop,
	knight,
	pawn
};

enum PieceColor{
	BLACK,
	WHITE
};

enum ErrorCode{
	MEM_FAIL,
	NULL_PARAM,
	INVALID_ENUM_PARAM,
	INVALID_INT_PARAM
};

typedef struct PieceStruct *Piece;

Piece piece_create(void);

void piece_destroy(Piece *p);

void piece_set_type(Piece p, enum PieceType pt);

enum PieceType piece_get_type(Piece p);

void piece_set_pos(Piece p, uint_fast8_t x, uint_fast8_t y);

uint_fast8_t piece_get_x(Piece p);

uint_fast8_t piece_get_y(Piece p);

void piece_set_color(Piece p, enum PieceColor c);

enum PieceColor piece_get_color(Piece p);

bool set_err_hndl(void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
