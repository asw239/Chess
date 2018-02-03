#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "../Piece/Piece.h"
#include <stdbool.h>

typedef struct BoardStruct *Board;

Board board_create(void);

void board_destroy(Board b);

Piece **board_get_arr(Board b);

uint_fast8_t board_get_size(void);

void board_set_turn(Board b, uint_fast8_t t);

void board_flip_turn(Board b);

uint_fast8_t board_get_turn(Board b);

Piece board_remove_piece(Board b, Piece p, bool destroy);

#endif
