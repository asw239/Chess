#ifndef DISPLAY_CLI_H
#define DISPLAY_CLI_H

#include <stdio.h>
#include "../Board/Board.h"
#include "../errors/ErrorCode.h"
#include "../errors/error_utilities.h"

void display_init_display(void);

void display_clear_screen(void);

void display_print_board(const Board b);

void display_print_menu(uint_fast8_t menu_width, uint_fast8_t menu_height);

char display_capture_menu(void);

void display_pause(void);

bool display_capture_move(FILE *s, uint_fast8_t *r_old, uint_fast8_t *c_old,
	uint_fast8_t *r_new, uint_fast8_t *c_new);

ErrFncPtr display_set_err_hndl(enum ErrorCode error_type,
	ErrFncPtr err_hndl);

#endif
