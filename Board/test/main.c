#include "../Board.h"
#include "../../display/display_cli.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	Board b = board_create();
	board_init_capture_list(b);

	Piece p1 = piece_create();
	piece_set_type(p1, knight);
	piece_set_color(p1, WHITE);
	piece_set_pos(p1, 0, 0);
	board_get_arr(b)[0][0] = p1;

	Piece p2 = piece_create();
	piece_set_type(p2, rook);
	piece_set_color(p2, BLACK);
	piece_set_pos(p2, 5, 3);
	board_get_arr(b)[5][3] = p2;

	Piece p3 = piece_create();
	piece_set_type(p3, king);
	piece_set_color(p3, WHITE);
	piece_set_pos(p3, 7, 7);
	board_get_arr(b)[7][7] = p3;

	init_display();
	print_board(b);

	board_remove_piece(b, &p1, false);
	board_remove_piece(b, &p2, false);
	board_remove_piece(b, &p3, false);

	print_board(b);
	for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
		if(board_get_capture_list(b, WHITE)[i])
			printf("%d ", piece_get_type(
				board_get_capture_list(b, WHITE)[i]));
	printf("\n");
	for(uint_fast8_t i = 0; i < PIECE_COUNT; i++)
		if(board_get_capture_list(b, BLACK)[i])
			printf("%d ", piece_get_type(
				board_get_capture_list(b, BLACK)[i]));
	printf("\n");

	board_destroy(&b);
	return 0;
}
