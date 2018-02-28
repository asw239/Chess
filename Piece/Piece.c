#include "Piece.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>
#include <stdio.h>

#define X_POS_MAX 7
#define Y_POS_MAX 7
#define FILE_NAME "Piece.c"

struct PieceStruct{
	enum PieceType *pt;
	uint_fast8_t *x;
	uint_fast8_t *y;
	enum PieceColor *color;
};

static void (*err_fnc_ptr)(enum ErrorCode err, const char *msg) = def_hndl;

Piece piece_create(void)
{
	#define FUNC_NAME "Piece piece_create(void)"

	Piece p;
	p = malloc(sizeof(*p));
	if(!p)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNC_NAME);

	p->pt = malloc(sizeof(enum PieceType));
	if(!p->pt)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNC_NAME);

	p->x = malloc(sizeof(uint_fast8_t));
	if(!p->x)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNC_NAME);

	p->y = malloc(sizeof(uint_fast8_t));
	if(!p->y)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNC_NAME);

	p->color = malloc(sizeof(enum PieceColor));
	if(!p->color)
		err_fnc_ptr(MEM_FAIL, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return p;
}

void piece_destroy(Piece *p)
{
	#define FUNC_NAME "void piece_destroy(Piece *p)"

	if(!p)
		err_fnc_ptr(NULL_PARAM,
			"In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	free((*p)->pt);
	free((*p)->x);
	free((*p)->y);
	free((*p)->color);
	free(*p);

	*p = NULL;
}

void piece_set_type(Piece p, enum PieceType pt)
{
	#define FUNC_NAME "void piece_set_type(Piece p, enum PieceType pt)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	if(pt != king && pt != queen && pt != rook && pt != bishop &&
		pt != knight && pt != pawn)
		err_fnc_ptr(INVALID_ENUM_PARAM,
			"In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	*(p->pt) = pt;
}

enum PieceType piece_get_type(Piece p)
{
	#define FUNC_NAME "enum PieceType piece_get_type(Piece p)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return *(p->pt);
}

void piece_set_pos(Piece p, uint_fast8_t x, uint_fast8_t y)
{
	#define FUNC_NAME "void piece_set_pos(Piece p, uint_fast8_t x," \
 		" uint_fast8_t y)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	if(x > X_POS_MAX || y > Y_POS_MAX)
		err_fnc_ptr(INVALID_INT_PARAM,
			"In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	*(p->x) = x;
	*(p->y) = y;
}

uint_fast8_t piece_get_x(Piece p)
{
	#define FUNC_NAME "uint_fast8_t piece_get_x(Piece p)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return *(p->x);
}

uint_fast8_t piece_get_y(Piece p)
{
	#define FUNC_NAME "uint_fast8_t piece_get_y(Piece p)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return *(p->y);
}

void piece_set_color(Piece p, enum PieceColor c)
{
	#define FUNC_NAME "void piece_set_color(Piece p, enum PieceColor c)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	if(c != WHITE && c != BLACK)
		err_fnc_ptr(INVALID_ENUM_PARAM,
			"In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	*(p->color) = c;
}

enum PieceColor piece_get_color(Piece p)
{
	#define FUNC_NAME "enum PieceColor piece_get_color(Piece p)"

	if(!p)
		err_fnc_ptr(NULL_PARAM, "In file " FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	return *(p->color);
}

bool set_err_hndl(void (*err_hndl)(enum ErrorCode err, const char *msg))
{
	if(err_hndl == NULL)
		return false;

	err_fnc_ptr = err_hndl;
	return true;
}
