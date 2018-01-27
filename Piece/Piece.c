#include "Piece.h"
#include <stdlib.h>

struct PieceStruct{
	enum PieceType *pt;
	Board brd;
};

Piece piece_create(void)
{
	Piece p;
	p = malloc(sizeof(*p));
	if(!p)
		; // callback for memory allocation fail

	p->pt = malloc(sizeof(enum PieceType));
	if(!p->pt)
		; // callback for memory allocation fail

	p->brd = malloc(sizeof(Board));
	if(!p->brd)
		; // callback for memory allocation fail

	return p;
}

void piece_destroy(Piece p)
{
	free(p->pt);
	free(p->brd);
	free(p);
}

void piece_set_type(Piece p, enum PieceType pt)
{
	*(p->pt) = pt;
}

enum PieceType piece_get_type(Piece p)
{
	return *(p->pt);
}

void piece_set_board(Piece p, Board b)
{
	p->brd = b;
}

Board piece_get_board(Piece p)
{
	return b->brd;
}

bool piece_validate_move(Piece p)
{
	switch(*(p->pt)){
	case king:

		break;
	case queen:

		break;
	case rook:

		break;
	case bishop:

		break;
	case knight:

		break;
	case pawn:

		break;
	}
}
