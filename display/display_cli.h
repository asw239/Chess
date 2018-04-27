#ifndef DISPLAY_CLI_H
#define DISPLAY_CLI_H

#include "../Board/Board.h"
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

void init_display(void);

void print_board(const Board b);

ErrFncPtr display_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl);

#endif
