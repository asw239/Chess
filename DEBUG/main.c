#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"
#include <stdio.h>
#include <stdlib.h>

void test_global(enum ErrorCode error_code, const char *msg);
void test_special(enum ErrorCode error_code, const char *msg);
void gl_test_move_validation(void);
void gl_test_start_board(void);
void board_test_link_move(void);

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

	Piece p1 = piece_create();
	piece_set_color(p1, WHITE);
	piece_set_type(p1, queen);
	board_link_piece(b, p1, 5, 5);
	Piece p2 = piece_create();
	piece_set_color(p2, BLACK);
	piece_set_type(p2, pawn);
	board_link_piece(b, p2, 5, 1);
	print_board(b);

	validate_move(b, 5, 5, 2, 1);
	board_move_piece(b, 5, 5, 2, 1);
	print_board(b);
}

void gl_test_start_board(void)
{
	Board b = generate_start_board();
	print_board(b);
}

void board_test_link_move(void)
{
	Board b = board_create();

	Piece p1 = piece_create();
	piece_set_type(p1, pawn);
	piece_set_color(p1, WHITE);
	board_link_piece(b, p1, 6, 0);
	Piece p2 = piece_create();
	piece_set_type(p2, pawn);
	piece_set_color(p2, BLACK);
	board_link_piece(b, p2, 1, 0);
	print_board(b);

	board_move_piece(b, 6, 0, 5, 0);
	board_move_piece(b, 1, 0, 2, 0);
	print_board(b);
}
