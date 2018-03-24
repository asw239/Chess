#ifndef DISPLAY_CLI_H
#define DISPLAY_CLI_H

#include "../Board/Board.h"
#include "../errors/ErrorCode.h"

void init_display(void);

void print_board(const Board b);

void display_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
