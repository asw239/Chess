#ifndef PIECE_H
#define PIECE_H

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

#endif
