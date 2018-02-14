#include "../display_cli.h"

int main(int argc, char *argv[])
{
	Board tst = board_create();
	Piece p_tst = piece_create();
	piece_set_type(p_tst, king);
	piece_set_color(p_tst, 0);
	board_get_arr(tst)[1][1] = p_tst;

	init_display();
	print_board(tst);
}
