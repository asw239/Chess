#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../Board/Board.h"
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

Board gl_generate_start_board(void);

bool gl_validate_move(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);

bool gl_check(const Board b, enum PieceColor c);

bool gl_mate(const Board b, enum PieceColor c);

bool gl_check_castle(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t c_new);

bool gl_check_en_passant(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);

ErrFncPtr gl_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl);

#endif
