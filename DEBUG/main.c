#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"
#include <stdio.h>
#include <stdlib.h>

void test_global(enum ErrorCode error_code, const char *msg);
void test_special(enum ErrorCode error_code, const char *msg);
void null_error(enum ErrorCode error_code, const char *msg);
void gl_test_move_validation(void);
void gl_test_start_board(void);
void board_test_link_move(void);
void board_test_move_capture(void);
void display_test_print_cap_list(void);
void board_test_unmoved_pieces(void);
void board_test_king_checked(void);
void gl_test_check(void);
void piece_test_copy(void);

int main(int argc, char *argv[])
{
	gl_test_check();
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

void null_error(enum ErrorCode error_code, const char *msg)
{
	;
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
	board_move_piece(&b, 5, 5, 2, 1);
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

	board_move_piece(&b, 6, 0, 5, 0);
	board_move_piece(&b, 1, 0, 2, 0);
	print_board(b);
}

void board_test_move_capture(void)
{
	Board b = generate_start_board();
	board_init_capture_list(b);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 0, 1, 2, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 2, 2, 4, 1);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 4, 1, 6, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 6, 0, 7, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 6, 3, 4, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 4, 3, 3, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 7, 4, 3, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 3, 0, 1, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	print_board(b);

	getchar();
	board_move_piece(&b, 1, 0, 0, 0);
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

void board_test_king_checked(void)
{
	Board b = generate_start_board();
	printf("white king checked? %d\n", board_is_king_checked(b, WHITE));
	printf("black king checked? %d\n", board_is_king_checked(b, BLACK));

	board_set_king_checked(b, WHITE, true);
	board_set_king_checked(b, BLACK, true);
	printf("white king checked? %d\n", board_is_king_checked(b, WHITE));
	printf("black king checked? %d\n", board_is_king_checked(b, BLACK));
}

void gl_test_check(void)
{
	Board b = board_create();
	board_init_capture_list(b);

	Piece p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	piece_set_pos(p, 7, 3);
	board_link_piece(b, p, 7, 3);

	p = piece_create();
	piece_set_type(p, QUEEN);
	piece_set_color(p, WHITE);
	piece_set_pos(p, 7, 2);
	board_link_piece(b, p, 7, 2);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, BLACK);
	piece_set_pos(p, 5, 2);
	board_link_piece(b, p, 5, 2);

	p = piece_create();
	piece_set_type(p, KNIGHT);
	piece_set_color(p, BLACK);
	piece_set_pos(p, 5, 7);
	board_link_piece(b, p, 5, 7);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, BLACK);
	piece_set_pos(p, 0, 3);
	board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_type(p, BISHOP);
	piece_set_color(p, BLACK);
	piece_set_pos(p, 0, 4);
	board_link_piece(b, p, 0, 4);

	p = piece_create();
	piece_set_type(p, KNIGHT);
	piece_set_color(p, BLACK);
	piece_set_pos(p, 0, 6);
	board_link_piece(b, p, 0, 6);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d\n", check(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 5, 2, 6, 2);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 7, 3, 6, 4);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 5, 7, 4, 5);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 7, 2, 4, 5);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 0, 4, 3, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 4, 5, 4, 6);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_move_piece(&b, 4, 6, 0, 6);

	for(int i = 0; i < 100; i++)
		printf("\n");
	printf("Check: %d, Mate: %d\n", check(b, WHITE), mate(b, WHITE));
	print_board(b);
	getchar();


	board_destroy(&b);
}

void piece_test_copy(void)
{
	Piece p_orig = piece_create();
	piece_set_type(p_orig, PAWN);
	piece_set_pos(p_orig, 0, 0);
	piece_set_color(p_orig, WHITE);
	Piece p_cpy = piece_create_copy(p_orig);

	printf("p_orig\n");
	printf("type: %d\n", piece_get_type(p_orig));
	printf("pos: (%d, %d)\n", piece_get_r(p_orig), piece_get_c(p_orig));
	printf("color: %d\n\n", piece_get_color(p_orig));

	printf("p_cpy\n");
	printf("type: %d\n", piece_get_type(p_cpy));
	printf("pos: (%d, %d)\n", piece_get_r(p_cpy), piece_get_c(p_cpy));
	printf("color: %d\n\n", piece_get_color(p_cpy));

	printf("----modifying p_cpy----\n\n");
	piece_set_type(p_cpy, QUEEN);
	piece_set_pos(p_cpy, 7, 7);
	piece_set_color(p_cpy, BLACK);

	printf("p_orig\n");
	printf("type: %d\n", piece_get_type(p_orig));
	printf("pos: (%d, %d)\n", piece_get_r(p_orig), piece_get_c(p_orig));
	printf("color: %d\n\n", piece_get_color(p_orig));

	printf("p_cpy\n");
	printf("type: %d\n", piece_get_type(p_cpy));
	printf("pos: (%d, %d)\n", piece_get_r(p_cpy), piece_get_c(p_cpy));
	printf("color: %d\n\n", piece_get_color(p_cpy));

	piece_destroy(&p_orig);
	piece_destroy(&p_cpy);
}
