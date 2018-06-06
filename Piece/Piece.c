#include "Piece.h"
#include "../errors/def_hndl.h"
#include <stdlib.h>
#include <stdio.h>

#define R_POS_MAX 7
#define C_POS_MAX 7

struct PieceStruct{
	enum PieceType *pt;
	uint_fast8_t *r;
	uint_fast8_t *c;
	enum PieceColor *color;
};

static ErrFncPtr err_fnc_arr[ERROR_CODE_COUNT] = {[GLOBAL_ERROR] = def_hndl};
static const char *FILE_NAME = "Piece.c";

Piece piece_create(void)
{
	const char *FUNC_NAME =
"Piece piece_create(void)";

	Piece p;
	p = malloc(sizeof(*p));
	if(!p){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	p->pt = calloc(1, sizeof(enum PieceType));
	if(!p->pt){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	p->r = calloc(1, sizeof(uint_fast8_t));
	if(!p->r){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	p->c = calloc(1, sizeof(uint_fast8_t));
	if(!p->c){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	p->color = calloc(1, sizeof(enum PieceColor));
	if(!p->color){
		call_error(err_fnc_arr, MEM_FAIL, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	return p;
}

Piece piece_create_copy(const Piece p)
{
	const char *FUNC_NAME =
"Piece piece_create_copy(const Piece p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return NULL;
	}

	Piece p_new = piece_create();

	*p_new->pt = *p->pt;
	*p_new->r = *p->r;
	*p_new->c = *p->c;
	*p_new->color = *p->color;

	return p_new;
}

void piece_destroy(Piece *p)
{
	const char *FUNC_NAME =
"void piece_destroy(Piece *p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	free((*p)->pt);
	free((*p)->r);
	free((*p)->c);
	free((*p)->color);
	free(*p);

	*p = NULL;
}

void piece_set_type(Piece p, enum PieceType pt)
{
	const char *FUNC_NAME =
"void piece_set_type(Piece p, enum PieceType pt)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(pt != KING && pt != QUEEN && pt != ROOK && pt != BISHOP &&
		pt != KNIGHT && pt != PAWN){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	*(p->pt) = pt;
}

enum PieceType piece_get_type(const Piece p)
{
	const char *FUNC_NAME =
"enum PieceType piece_get_type(const Piece p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return 0;
	}

	return *(p->pt);
}

void piece_set_pos(Piece p, uint_fast8_t r, uint_fast8_t c)
{
	const char *FUNC_NAME =
"void piece_set_pos(Piece p, uint_fast8_t r, uint_fast8_t c)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(r > R_POS_MAX || c > C_POS_MAX){
		call_error(err_fnc_arr, INVALID_INT_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	*(p->r) = r;
	*(p->c) = c;
}

uint_fast8_t piece_get_r(const Piece p)
{
	const char *FUNC_NAME =
"uint_fast8_t piece_get_r(const Piece p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return 0;
	}

	return *(p->r);
}

uint_fast8_t piece_get_c(const Piece p)
{
	const char *FUNC_NAME =
"uint_fast8_t piece_get_c(const Piece p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return 0;
	}

	return *(p->c);
}

void piece_set_color(Piece p, enum PieceColor c)
{
	const char *FUNC_NAME =
"void piece_set_color(Piece p, enum PieceColor c)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(c != WHITE && c != BLACK){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	*(p->color) = c;
}

enum PieceColor piece_get_color(const Piece p)
{
	const char *FUNC_NAME =
"enum PieceColor piece_get_color(const Piece p)";

	if(!p){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return 0;
	}

	return *(p->color);
}

ErrFncPtr piece_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)
{
	const char *FUNC_NAME =
"ErrFncPtr piece_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl)";

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
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
