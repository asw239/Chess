#include <stdio.h>
#include "../Board.h"

const char *turn_to_text(enum PieceColor c);

int main(int argc, char *argv[]){
	Board b = board_create();
	board_set_turn(b, BLACK);
	printf("current turn: %s\n", turn_to_text(board_get_turn(b)));

	board_flip_turn(b);
	printf("flip turn: %s\n", turn_to_text(board_get_turn(b)));
	board_flip_turn(b);
	printf("flip turn: %s\n", turn_to_text(board_get_turn(b)));

	Piece p = piece_create();
	piece_set_type(p, queen);
	piece_set_pos(p, 5, 1);
	piece_set_color(p, WHITE);
	board_get_arr(b)[5][1] = p;
	board_remove_piece(b, &p, true);
	printf("%p\n", p);

	board_destroy(&b);

	return 0;
}

const char *turn_to_text(enum PieceColor c)
{
	return c == WHITE ? "WHITE" : "BLACK";
}
