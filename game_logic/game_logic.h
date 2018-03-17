#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "../Board/Board.h"

Board generate_start_board(void);

void validate_move(const Board b, const Piece p, uint_fast8_t x,
	uint_fast8_t y);

void gl_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
