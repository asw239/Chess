#include "Piece.h"
#include <stdlib.h>

struct PieceStruct{
	enum PieceType *pt;
	uint_fast8_t *x;
	uint_fast8_t *y;
	uint_fast8_t *color;
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

	p->x = malloc(sizeof(uint_fast8_t));
	if(!p->x)
		; // callback for memory allocation fail

	p->y = malloc(sizeof(uint_fast8_t));
	if(!p->y)
		; // callback for memory allocation fail

	p->color = malloc(sizeof(uint_fast8_t));
	if(!p->color)
		; // callback for memory allocation fail

	return p;
}

void piece_destroy(Piece p)
{
	free(p->pt);
	free(p->x);
	free(p->y);
	free(p->color);
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

void piece_set_pos(Piece p, uint_fast8_t x, uint_fast8_t y)
{
	*(p->x) = x;
	*(p->y) = y;
}

uint_fast8_t piece_get_x(Piece p)
{
	return *(p->x);
}

uint_fast8_t piece_get_y(Piece p)
{
	return *(p->y);
}

void piece_set_color(Piece p, uint_fast8_t c)
{
	*(p->color) = c;
}

uint_fast8_t piece_get_color(Piece p)
{
	return *(p->color);
}
