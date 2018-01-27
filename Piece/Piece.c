#include "Piece.h"
#include "../Board/Board.h"
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
