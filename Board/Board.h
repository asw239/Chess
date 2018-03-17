#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdbool.h>
#include "../Piece/Piece.h"
#include "../errors/ErrorCode.h"

typedef struct BoardStruct *Board;

Board board_create(void);

void board_destroy(Board *b);

Piece **board_get_arr(const Board b);

uint_fast8_t board_get_size(void);

void board_set_turn(Board b, enum PieceColor c);

void board_flip_turn(Board b);

enum PieceColor board_get_turn(const Board b);

Piece board_remove_piece(Board b, Piece *p, bool destroy);

void board_init_capture_list(Board b);

Piece *board_get_capture_list(const Board b, enum PieceColor c);

void board_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg));

#endif
