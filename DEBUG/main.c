#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"
#include <stdio.h>
#include <stdlib.h>

void test_global(enum ErrorCode error_code, const char *msg);
void test_special(enum ErrorCode error_code, const char *msg);
void gl_test_move_validation(void);
void gl_test_start_board(void);
void board_test_link_move(void);
void board_test_move_capture(void);
void display_test_print_cap_list(void);
void board_test_unmoved_pieces(void);

int main(int argc, char *argv[])
{
	board_test_unmoved_pieces();
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
	piece_set_type(p1, QUEEN);
	board_link_piece(b, p1, 5, 5);
	Piece p2 = piece_create();
	piece_set_color(p2, BLACK);
	piece_set_type(p2, PAWN);
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
	piece_set_type(p1, PAWN);
	piece_set_color(p1, WHITE);
	board_link_piece(b, p1, 6, 0);
	Piece p2 = piece_create();
	piece_set_type(p2, PAWN);
	piece_set_color(p2, BLACK);
	board_link_piece(b, p2, 1, 0);
	print_board(b);

	board_move_piece(b, 6, 0, 5, 0);
	board_move_piece(b, 1, 0, 2, 0);
	print_board(b);
}

void board_test_move_capture(void)
{
	Board b = generate_start_board();
	board_init_capture_list(b);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 0, 1, 2, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 2, 2, 4, 1);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 4, 1, 6, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 6, 0, 7, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 6, 3, 4, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 4, 3, 3, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 7, 4, 3, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 3, 0, 1, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(b, 1, 0, 0, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);
}

void display_test_print_cap_list(void)
{
	Board b = generate_start_board();
	board_init_capture_list(b);

	for(uint_fast8_t i = 0; i < PIECE_COUNT; i++){
		Piece tmp = piece_create();
		piece_set_type(tmp, KING);
		piece_set_color(tmp, WHITE);
		board_get_capture_list(b, WHITE)[i] = tmp;
	}
	for(uint_fast8_t i = 0; i < PIECE_COUNT; i++){
		Piece tmp = piece_create();
		piece_set_type(tmp, KING);
		piece_set_color(tmp, BLACK);
		board_get_capture_list(b, BLACK)[i] = tmp;
	}

	print_board(b);
}

void board_test_unmoved_pieces(void)
{
	Board b = generate_start_board();
	printf("W_L_ROOK moved? %d\n", board_has_piece_moved(b, W_L_ROOK));

	board_set_piece_moved(b, W_L_ROOK, true);
	printf("W_L_ROOK moved? %d\n", board_has_piece_moved(b, W_L_ROOK));
}
