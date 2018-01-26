#include "Board.h"
#include <stdlib.h>

struct BoardStruct{
	int **board_arr;
};

Board board_create()
{
	Board b;
	b = malloc(sizeof(*b));

	if(!b)
		; // callback for memory allocation fail

	return b;
}
