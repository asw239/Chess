#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
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

	board_set_turn(tst_brd, 1);
	printf("turn: ");
	printf("%" PRIuFAST8, board_get_turn(tst_brd));
	printf("\n");

	board_flip_turn(tst_brd);
	printf("flip turn: ");
	printf("%" PRIuFAST8, board_get_turn(tst_brd));
	printf("\n");

	Piece p = piece_create();
	tst_brd_arr[1][1] = p;
	piece_set_pos(p, 1, 1);
	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < board_get_size(); j++){
			if(tst_brd_arr[i][j] == NULL)
				printf("- ");
			else
				printf("x ");
		}
		printf("\n");
	}
	printf("\n");
	board_remove_piece(tst_brd, p, true);
	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < board_get_size(); j++){
			if(tst_brd_arr[i][j] == NULL)
				printf("- ");
			else
				printf("x ");
		}
		printf("\n");
	}

	board_destroy(tst_brd);
	return 0;
}
