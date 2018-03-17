#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"
#include <stdio.h>
#include <stdlib.h>

void test_global(enum ErrorCode error_code, const char *msg);
void test_special(enum ErrorCode error_code, const char *msg);
void gl_test_move_validation(void);
void gl_test_start_board(void);

int main(int argc, char *argv[])
{
	gl_test_move_validation();
	return 0;
}

void test_global(enum ErrorCode error_code, const char *msg)
{
	printf("han dead bam!\n");
	exit(0);
}

void test_special(enum ErrorCode error_code, const char *msg)
{
	printf("special boyo\n");
	exit(0);
}

void gl_test_move_validation(void)
{
	Board b = board_create();

	Piece p = piece_create();
	piece_set_type(p, pawn);
	piece_set_color(p, WHITE);
	piece_set_pos(p, 1, 1);
	board_get_arr(b)[1][1] = p;

	Piece p2 = piece_create();
	piece_set_type(p2, knight);
	piece_set_color(p2, BLACK);
	piece_set_pos(p2, 0, 2);
	board_get_arr(b)[0][2] = p2;

	print_board(b);

	validate_move(b, p, 0, 2);
	piece_set_pos(p, 0, 2);
	board_get_arr(b)[0][2] = p;
	board_get_arr(b)[1][1] = NULL;

	print_board(b);
}

void gl_test_start_board(void)
{
	Board b = generate_start_board();
	print_board(b);
}
