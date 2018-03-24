#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "../Piece/Piece.h"
#include "../errors/ErrorCode.h"

typedef struct BoardStruct *Board;

Board board_create(void);

void board_destroy(Board *b);

void board_link_piece(Board b, Piece p, uint_fast8_t r, uint_fast8_t c);

void board_move_piece(Board b, uint_fast8_t r_old, uint_fast8_t c_old,
	uint_fast8_t r_new, uint_fast8_t c_new);

Piece board_get_piece(Board b, uint_fast8_t r, uint_fast8_t c);

uint_fast8_t board_get_size(void);

void board_set_turn(Board b, enum PieceColor c);

void board_flip_turn(Board b);

enum PieceColor board_get_turn(const Board b);

Piece board_remove_piece(Board b, uint_fast8_t r, uint_fast8_t c, bool destroy);

void board_init_capture_list(Board b);

Piece *board_get_capture_list(const Board b, enum PieceColor c);

void board_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
