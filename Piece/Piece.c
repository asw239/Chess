#include "Piece.h"
#include <stdlib.h>
#include <stdint.h>

struct PieceStruct{
	enum PieceType *pt;
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

	return p;
}

void piece_destroy(Piece p)
{
	free(p->pt);
	free(p);
}
