#include <stdio.h>
#include <stdint.h>
#include "../Board.h"
#include "../../Piece/Piece.h"

int main(int argc, char *argv[]){
	Board tst_brd = board_create();

	Piece **tst_brd_arr = board_get_arr(tst_brd);
	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < board_get_size(); j++){
			if(tst_brd_arr[i][j] == NULL)
				printf("- ");
		}
		printf("\n");
	}

	board_destroy(tst_brd);
	return 0;
}
