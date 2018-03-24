#include "Piece.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>
#include <stdio.h>

#define R_POS_MAX 7
#define C_POS_MAX 7
#define FILE_NAME "Piece.c"

struct PieceStruct{
	enum PieceType *pt;
	uint_fast8_t *r;
	uint_fast8_t *c;
	enum PieceColor *color;
};

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

Piece piece_create(void)
{
	#define FUNC_NAME "Piece piece_create(void)"

	Piece p;
	p = malloc(sizeof(*p));
	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	p->pt = calloc(1, sizeof(enum PieceType));
	if(!p->pt){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	p->r = calloc(1, sizeof(uint_fast8_t));
	if(!p->r){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	p->c = calloc(1, sizeof(uint_fast8_t));
	if(!p->c){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	p->color = calloc(1, sizeof(enum PieceColor));
	if(!p->color){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return p;
}

void piece_destroy(Piece *p)
{
	#define FUNC_NAME "void piece_destroy(Piece *p)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	free((*p)->pt);
	free((*p)->r);
	free((*p)->c);
	free((*p)->color);
	free(*p);

	*p = NULL;
}

void piece_set_type(Piece p, enum PieceType pt)
{
	#define FUNC_NAME "void piece_set_type(Piece p, enum PieceType pt)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(pt != KING && pt != QUEEN && pt != ROOK && pt != BISHOP &&
		pt != KNIGHT && pt != PAWN){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	*(p->pt) = pt;
}

enum PieceType piece_get_type(const Piece p)
{
	#define FUNC_NAME "enum PieceType piece_get_type(Piece p)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return *(p->pt);
}

void piece_set_pos(Piece p, uint_fast8_t r, uint_fast8_t c)
{
	#define FUNC_NAME "void piece_set_pos(Piece p, uint_fast8_t r," \
		" uint_fast8_t c)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(r > R_POS_MAX || c > C_POS_MAX){
		if(!err_fnc_arr[INVALID_INT_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_INT_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_INT_PARAM](INVALID_INT_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	*(p->r) = r;
	*(p->c) = c;
}

uint_fast8_t piece_get_r(const Piece p)
{
	#define FUNC_NAME "uint_fast8_t piece_get_r(Piece p)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return *(p->r);
}

uint_fast8_t piece_get_c(const Piece p)
{
	#define FUNC_NAME "uint_fast8_t piece_get_c(Piece p)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return *(p->c);
}

void piece_set_color(Piece p, enum PieceColor c)
{
	#define FUNC_NAME "void piece_set_color(Piece p, enum PieceColor c)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	if(c != WHITE && c != BLACK){
		if(!err_fnc_arr[INVALID_ENUM_PARAM])
			err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[INVALID_ENUM_PARAM](INVALID_ENUM_PARAM,
				"In file " FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	*(p->color) = c;
}

enum PieceColor piece_get_color(const Piece p)
{
	#define FUNC_NAME "enum PieceColor piece_get_color(Piece p)"

	if(!p){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

	return *(p->color);
}

void piece_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg))
{
	#define FUNC_NAME "void piece_set_err_hndl(enum ErrorCode error_type, "\
		"void (*err_hndl)(enum ErroCode err, const char *msg))"

	if(!(
		error_type != GLOBAL_ERROR
		||
		error_type != MEM_FAIL
		||
		error_type != NULL_PARAM
		||
		error_type != INVALID_ENUM_PARAM
		||
		error_type != INVALID_INT_PARAM
	))
		err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	err_fnc_arr[error_type] = err_hndl;
}
