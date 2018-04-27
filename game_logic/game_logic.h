#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../Board/Board.h"
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

Board generate_start_board(void);

void validate_move(const Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);

ErrFncPtr gl_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl);

#endif
