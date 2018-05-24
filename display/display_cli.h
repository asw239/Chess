#ifndef DISPLAY_CLI_H
#define DISPLAY_CLI_H

#include "../Board/Board.h"
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

void init_display(void);

void clear_screen(void);

void print_board(const Board b);

void print_menu(uint_fast8_t menu_width, uint_fast8_t menu_height);

char capture_menu(void);

void pause(void);

bool capture_move(uint_fast8_t *r_old, uint_fast8_t *c_old, uint_fast8_t *r_new,
	uint_fast8_t *c_new);

ErrFncPtr display_set_err_hndl(enum ErrorCode error_type, ErrFncPtr err_hndl);

#endif
