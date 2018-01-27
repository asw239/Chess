#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "../Piece/Piece.h"

typedef struct BoardStruct *Board;

Board board_create(void);

void board_destroy(Board b);

Piece **board_get_arr(Board b);

uint_fast8_t board_get_size(void);

#endif
