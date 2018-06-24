#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"
#include "../game_mode/menu.h"
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
void piece_test_copy(void);
void board_test_copy(void);
void board_test_destroy_error(void);
void gl_test_check_1(void);
void gl_test_check_2(void);
void gl_test_check_time_complexity(void);
void gl_test_mate(void);
void gl_test_mate2(void);
void gl_test_mate3(void);
void gl_test_mate4(void);
//void gl_test_castle(enum PieceColor c);
//void gl_test_castle2(enum PieceColor c, bool left_castle);
//void gl_test_castle3(enum PieceColor c, bool left_castle);
//void gl_test_castle4(enum PieceColor c, bool left_castle, bool queen,
	//bool bishop, bool knight);
//void gl_test_castle5(enum PieceColor c);
//void gl_test_castle6(enum PieceColor c, bool left_castle);
void gl_move_piece_castle(void);
void gl_move_piece_castle2(void);
void gl_move_piece_en_passant(void);
void board_create_copy_en_passant(void);
void gl_test_check_en_passant(void);
void display_test_print_menu(void);
void test_entry_point(void);
void debug_error_1(void);
void debug_error_1_1(void);
void debug_error_2(void);

int main(int argc, char *argv[])
{
	debug_error_2();
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

	Piece p3 = piece_create();
	piece_set_color(p3, BLACK);
	piece_set_type(p3, PAWN);
	board_link_piece(b, p3, 5, 3);

	display_print_board(b);
	getchar();

	gl_move_piece(b, 5, 5, 5, 1);
}

void gl_test_start_board(void)
{
	Board b = gl_generate_start_board();
	display_print_board(b);
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
	display_print_board(b);

	gl_move_piece(b, 6, 0, 5, 0);
	gl_move_piece(b, 1, 0, 2, 0);
	display_print_board(b);
}

void board_test_move_capture(void)
{
	Board b = gl_generate_start_board();
	board_init_capture_list(b);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 0, 1, 2, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 2, 2, 4, 1);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 4, 1, 6, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 6, 0, 7, 2);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 6, 3, 4, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 4, 3, 3, 3);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 7, 4, 3, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 3, 0, 1, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);

	getchar();
	gl_move_piece(b, 1, 0, 0, 0);
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	display_print_board(b);
}

void display_test_print_cap_list(void)
{
	Board b = gl_generate_start_board();
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

	display_print_board(b);
}

void board_test_unmoved_pieces(void)
{
	Board b = gl_generate_start_board();

	for(int i = 0; i < 50; i++)
		printf("\n");
	display_print_board(b);
	printf("W_L_ROOK moved: %s\n",
		board_has_piece_moved(b, W_L_ROOK) ? "true" : "false");
	printf("W_R_ROOK moved: %s\n",
		board_has_piece_moved(b, W_R_ROOK) ? "true" : "false");
	printf("W_KING moved: %s\n",
		board_has_piece_moved(b, W_KING) ? "true" : "false");
	printf("B_L_ROOK moved: %s\n",
		board_has_piece_moved(b, B_L_ROOK) ? "true" : "false");
	printf("B_R_ROOK moved: %s\n",
		board_has_piece_moved(b, B_R_ROOK) ? "true" : "false");
	printf("B_KING moved: %s\n",
		board_has_piece_moved(b, B_KING) ? "true" : "false");
	getchar();

	gl_move_piece(b, 1, 0, 3, 0);
	gl_move_piece(b, 1, 7, 3, 7);
	gl_move_piece(b, 1, 3, 3, 3);
	gl_move_piece(b, 6, 0, 4, 0);
	gl_move_piece(b, 6, 7, 4, 7);
	gl_move_piece(b, 6, 3, 4, 3);

	for(int i = 0; i < 50; i++)
		printf("\n");
	display_print_board(b);
	printf("W_L_ROOK moved: %s\n",
		board_has_piece_moved(b, W_L_ROOK) ? "true" : "false");
	printf("W_R_ROOK moved: %s\n",
		board_has_piece_moved(b, W_R_ROOK) ? "true" : "false");
	printf("W_KING moved: %s\n",
		board_has_piece_moved(b, W_KING) ? "true" : "false");
	printf("B_L_ROOK moved: %s\n",
		board_has_piece_moved(b, B_L_ROOK) ? "true" : "false");
	printf("B_R_ROOK moved: %s\n",
		board_has_piece_moved(b, B_R_ROOK) ? "true" : "false");
	printf("B_KING moved: %s\n",
		board_has_piece_moved(b, B_KING) ? "true" : "false");
	getchar();

	gl_move_piece(b, 0, 0, 2, 0);
	gl_move_piece(b, 7, 3, 6, 3);

	for(int i = 0; i < 50; i++)
		printf("\n");
	display_print_board(b);
	printf("W_L_ROOK moved: %s\n",
		board_has_piece_moved(b, W_L_ROOK) ? "true" : "false");
	printf("W_R_ROOK moved: %s\n",
		board_has_piece_moved(b, W_R_ROOK) ? "true" : "false");
	printf("W_KING moved: %s\n",
		board_has_piece_moved(b, W_KING) ? "true" : "false");
	printf("B_L_ROOK moved: %s\n",
		board_has_piece_moved(b, B_L_ROOK) ? "true" : "false");
	printf("B_R_ROOK moved: %s\n",
		board_has_piece_moved(b, B_R_ROOK) ? "true" : "false");
	printf("B_KING moved: %s\n",
		board_has_piece_moved(b, B_KING) ? "true" : "false");
	getchar();
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

void board_test_copy(void)
{
	Board b = gl_generate_start_board();
	Board b_cpy = board_create_copy(b);

	printf("original board\n");
	display_print_board(b);
	printf("copied board\n");
	display_print_board(b_cpy);

	board_destroy(&b);
	board_destroy(&b_cpy);
}

void board_test_destroy_error(void)
{
	board_set_err_hndl(NULL_PARAM, test_special);
	board_destroy(0);
}

void gl_test_check_1(void)
{
	Board b = board_create();
	Piece p;

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 7);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 5, 6);

	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	getchar();

	gl_move_piece(b, 5, 6, 6, 6);
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	getchar();

	gl_move_piece(b, 7, 7, 6, 6);
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	getchar();

	board_destroy(&b);
}

void gl_test_check_2(void)
{
	Board b = gl_generate_start_board();
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, BLACK));
	getchar();

	gl_move_piece(b, 6, 3, 5, 3);
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, BLACK));
	getchar();

	gl_move_piece(b, 1, 2, 3, 2);
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, BLACK));
	getchar();

	gl_move_piece(b, 7, 4, 3, 0);
	for(int i = 0; i < 100; i++) printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, BLACK));
	getchar();

	board_destroy(&b);
}

void gl_test_check_time_complexity(void)
{
	//lol small delay at 100,000 check attempts, dont even sweat it
	Board b = gl_generate_start_board();
	for(int i = 0; i < 100000; i++)
		gl_check(b, BLACK);

	board_destroy(&b);
}

void gl_test_mate(void)
{
	Board b = board_create();
	Piece p;

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 7, 7);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 5, 6);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 5, 5);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 6);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 6);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 5);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	gl_move_piece(b, 5, 6, 6, 6);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	board_destroy(&b);
}

void gl_test_mate2(void)
{
	Board b = board_create();
	Piece p;

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 5, 0);

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 6, 1);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	gl_move_piece(b, 5, 0, 7, 0);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	board_destroy(&b);
}

void gl_test_mate3(void)
{
	Board b = board_create();
	Piece p;

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 6);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 7);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 6);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 5);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 6);

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 0);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	gl_move_piece(b, 0, 0, 7, 0);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	board_destroy(&b);
}

void gl_test_mate4(void)
{
	Board b = board_create();
	board_init_capture_list(b);
	Piece p;

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 6);

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 5);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 6);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 5);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 6);

	p = piece_create();
	piece_set_type(p, BISHOP);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_type(p, QUEEN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 1, 1);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	gl_move_piece(b, 1, 1, 6, 6);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	printf("check: %d\n", gl_check(b, WHITE));
	printf("mate: %d\n", gl_mate(b, WHITE));
	getchar();

	board_destroy(&b);
}

/*void gl_test_castle(enum PieceColor c)
{
	//validates the function check_castle_attempt
	//lowkey validates all of them

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE){
		printf("valid castle attempt from [7, 3] to [7, 4]? %d\n",
			check_castle(b, 7, 3, 4));
		printf("valid castle attempt from [7, 3] to [7, 2]? %d\n",
			check_castle(b, 7, 3, 2));
		printf("valid castle attempt from [7, 3] to [7, 1]? %d\n",
			check_castle(b, 7, 3, 1));
		printf("valid castle attempt from [7, 3] to [7, 5]? %d\n",
			check_castle(b, 7, 3, 5));
	}else{
		printf("valid castle attempt from [0, 3] to [0, 4]? %d\n",
			check_castle(b, 0, 3, 4));
		printf("valid castle attempt from [0, 3] to [0, 2]? %d\n",
			check_castle(b, 0, 3, 2));
		printf("valid castle attempt from [0, 3] to [0, 1]? %d\n",
			check_castle(b, 0, 3, 1));
		printf("valid castle attempt from [0, 3] to [0, 5]? %d\n",
			check_castle(b, 0, 3, 5));
	}
	getchar();

	board_destroy(&b);
}

void gl_test_castle2(enum PieceColor c, bool left_castle)
{
	//validates the function check_castle_king_moved

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	if(c == WHITE)
		gl_move_piece(b, 7, 3, 6, 3);
	else
		gl_move_piece(b, 0, 3, 1, 3);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	if(c == WHITE)
		gl_move_piece(b, 6, 3, 7, 3);
	else
		gl_move_piece(b, 1, 3, 0, 3);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE && left_castle){
		printf("valid castle attempt from [7, 3] to [7, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 1));

	}else if(c == WHITE && !left_castle){
		printf("valid castle attempt from [7, 3] to [7, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 5));

	}else if(c == BLACK && left_castle){
		printf("valid castle attempt from [0, 3] to [0, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 1));

	}else if(c == BLACK && !left_castle){
		printf("valid castle attempt from [0, 3] to [0, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 5));
	}
	getchar();

	board_destroy(&b);
}

void gl_test_castle3(enum PieceColor c, bool left_castle)
{
	//validates the function check_castle_rook_moved

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	if(c == WHITE && left_castle)
		gl_move_piece(b, 7, 0, 0, 0);

	else if(c == WHITE && !left_castle)
		gl_move_piece(b, 7, 7, 0, 7);

	else if(c == BLACK && left_castle)
		gl_move_piece(b, 0, 0, 7, 0);

	else if(c == BLACK && !left_castle)
		gl_move_piece(b, 0, 7, 7, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	if(c == WHITE && left_castle)
		gl_move_piece(b, 0, 0, 7, 0);

	else if(c == WHITE && !left_castle)
		gl_move_piece(b, 0, 7, 7, 7);

	else if(c == BLACK && left_castle)
		gl_move_piece(b, 7, 0, 0, 0);

	else if(c == BLACK && !left_castle)
		gl_move_piece(b, 7, 7, 0, 7);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE && left_castle){
		printf("valid castle attempt from [7, 3] to [7, 5]? %d\n",
			check_castle(b, 7, 3, 5));
		printf("valid castle attempt from [7, 3] to [7, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 1));

	}else if(c == WHITE && !left_castle){
		printf("valid castle attempt from [7, 3] to [7, 1]? %d\n",
			check_castle(b, 7, 3, 1));
		printf("valid castle attempt from [7, 3] to [7, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 5));

	}else if(c == BLACK && left_castle){
		printf("valid castle attempt from [0, 3] to [0, 5]? %d\n",
			check_castle(b, 0, 3, 5));
		printf("valid castle attempt from [0, 3] to [0, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 1));

	}else if(c == BLACK && !left_castle){
		printf("valid castle attempt from [0, 3] to [0, 1]? %d\n",
			check_castle(b, 0, 3, 1));
		printf("valid castle attempt from [0, 3] to [0, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 5));
	}
	getchar();

	board_destroy(&b);
}

void gl_test_castle4(enum PieceColor c, bool left_castle, bool queen,
	bool bishop, bool knight)
{
	//validates the function check_castle_pieces_inbetween

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	if(bishop){
		p = piece_create();
		piece_set_color(p, c);
		piece_set_type(p, BISHOP);
		(c == WHITE)
			? board_link_piece(b, p, 7, 2)
			: board_link_piece(b, p, 0, 2);

		p = piece_create();
		piece_set_color(p, c);
		piece_set_type(p, BISHOP);
		(c == WHITE)
			? board_link_piece(b, p, 7, 5)
			: board_link_piece(b, p, 0, 5);
	}

	if(knight){
		p = piece_create();
		piece_set_color(p, c);
		piece_set_type(p, KNIGHT);
		(c == WHITE)
			? board_link_piece(b, p, 7, 1)
			: board_link_piece(b, p, 0, 1);

		p = piece_create();
		piece_set_color(p, c);
		piece_set_type(p, KNIGHT);
		(c == WHITE)
			? board_link_piece(b, p, 7, 6)
			: board_link_piece(b, p, 0, 6);
	}

	if(queen){
		p = piece_create();
		piece_set_color(p, c);
		piece_set_type(p, QUEEN);
		(c == WHITE)
			? board_link_piece(b, p, 7, 4)
			: board_link_piece(b, p, 0, 4);

	}

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE && left_castle){
		printf("valid castle attempt from [7, 3] to [7, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 1));

	}else if(c == WHITE && !left_castle){
		printf("valid castle attempt from [7, 3] to [7, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 5));

	}if(c == BLACK && left_castle){
		printf("valid castle attempt from [0, 3] to [0, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 1));

	}else if(c == BLACK && !left_castle){
		printf("valid castle attempt from [0, 3] to [0, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 5));
	}

	getchar();
	board_destroy(&b);
}

void gl_test_castle5(enum PieceColor c)
{
	//validates function check_castle_king_in_check

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	p = piece_create();
	piece_set_type(p, QUEEN);
	if( c== WHITE){
		piece_set_color(p, BLACK);
		board_link_piece(b, p, 2, 3);
	}else{
		piece_set_color(p, WHITE);
		board_link_piece(b, p, 5, 3);
	}

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE){
		printf("valid castle attempt from [7, 3] to [7, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 1));
	}else{
		printf("valid castle attempt from [0, 3] to [0, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 1));
	}
	getchar();

	board_destroy(&b);
}

void gl_test_castle6(enum PieceColor c, bool left_castle)
{
	//validates function check_castle_through_check

	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, null_error);

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, KING);
	(c == WHITE)
		? board_link_piece(b, p, 7, 3)
		: board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 0)
		: board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, c);
	piece_set_type(p, ROOK);
	(c == WHITE)
		? board_link_piece(b, p, 7, 7)
		: board_link_piece(b, p, 0, 7);

	p = piece_create();
	piece_set_type(p, QUEEN);
	if(c== WHITE && left_castle){
		piece_set_color(p, BLACK);
		board_link_piece(b, p, 2, 2);

	}else if(c == WHITE && !left_castle){
		piece_set_color(p, BLACK);
		board_link_piece(b, p, 2, 4);

	}else if(c== BLACK && left_castle){
		piece_set_color(p, WHITE);
		board_link_piece(b, p, 5, 2);

	}else if(c == BLACK && !left_castle){
		piece_set_color(p, WHITE);
		board_link_piece(b, p, 5, 4);
	}

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	if(c == WHITE && left_castle){
		printf("valid castle attempt from [7, 3] to [7, 5]? %d\n",
			check_castle(b, 7, 3, 5));
		printf("valid castle attempt from [7, 3] to [7, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 1));

	}else if(c == WHITE && !left_castle){
		printf("valid castle attempt from [7, 3] to [7, 1]? %d\n",
			check_castle(b, 7, 3, 1));
		printf("valid castle attempt from [7, 3] to [7, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 7, 3, 5));

	}else if(c == BLACK && left_castle){
		printf("valid castle attempt from [0, 3] to [0, 5]? %d\n",
			check_castle(b, 0, 3, 5));
		printf("valid castle attempt from [0, 3] to [0, 1]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 1));

	}else if(c == BLACK && !left_castle){
		printf("valid castle attempt from [0, 3] to [0, 1]? %d\n",
			check_castle(b, 0, 3, 1));
		printf("valid castle attempt from [0, 3] to [0, 5]? ");
		fflush(stdout);
		printf("%d\n", check_castle(b, 0, 3, 5));
	}

	getchar();

	board_destroy(&b);

}*/

void gl_move_piece_castle(void)
{
	// tests valid castle execution

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 0);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 3);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 7, 3, 7, 1);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	board_destroy(&b);
}

void gl_move_piece_castle2(void)
{
	// tests in-valid castle execution

	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, null_error);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, null_error);

	Board b = board_create();
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 0);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 3);

	p = piece_create();
	piece_set_type(p, QUEEN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 0, 3);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 7, 3, 7, 1);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	board_destroy(&b);
}

void gl_move_piece_en_passant(void)
{
	//tests setting and un-setting of the en_pessant_pawn

	Board b = gl_generate_start_board();

	Piece en_passant_pawn;

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 6, 4, 4, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 1, 2, 3, 2);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 6, 7, 5, 7);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 1, 6, 3, 6);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 7, 4, 5, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	board_destroy(&b);
}

void board_create_copy_en_passant(void)
{
	//tests board copy creation in terms of en passant

	Board b = gl_generate_start_board();

	Piece en_passant_pawn;

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b, 6, 4, 4, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	en_passant_pawn = board_get_en_passant_pawn(b);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	Board b_cpy = board_create_copy(b);
	board_destroy(&b);
	gl_move_piece(b_cpy, 1, 2, 3, 2);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b_cpy);
	en_passant_pawn = board_get_en_passant_pawn(b_cpy);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b_cpy, 6, 7, 5, 7);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b_cpy);
	en_passant_pawn = board_get_en_passant_pawn(b_cpy);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b_cpy, 1, 6, 3, 6);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b_cpy);
	en_passant_pawn = board_get_en_passant_pawn(b_cpy);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	gl_move_piece(b_cpy, 7, 4, 5, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b_cpy);
	en_passant_pawn = board_get_en_passant_pawn(b_cpy);
	if(en_passant_pawn){
		printf("en passant pawn detected: [%d, %d]\n",
			piece_get_r(en_passant_pawn),
			piece_get_c(en_passant_pawn));
	}
	getchar();

	board_destroy(&b_cpy);
}

void gl_test_check_en_passant(void)
{
	/*
	 * validates functions check_en_passant_attempt and 
	 * check_en_passant_pawn
	 */

	Board b = board_create();
	board_init_capture_list(b);
	board_set_piece_moved(b, W_L_ROOK, false);
	board_set_piece_moved(b, W_R_ROOK, false);
	board_set_piece_moved(b, W_KING, false);
	board_set_piece_moved(b, B_L_ROOK, false);
	board_set_piece_moved(b, B_R_ROOK, false);
	board_set_piece_moved(b, B_KING, false);

	Piece p;

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 6, 3);

	p = piece_create();
	piece_set_type(p, PAWN);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 1, 4);

	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 6, 3, 4, 3);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 4, 3, 3, 3);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 1, 4, 3, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	gl_move_piece(b, 3, 3, 2, 4);
	for(int i = 0; i < 100; i++)
		printf("\n");
	display_print_board(b);
	getchar();

	board_destroy(&b);
}

void display_test_print_menu(void)
{
	display_clear_screen();
	display_print_menu(60, 20);
	display_capture_menu();
}

void test_entry_point(void)
{
	chess_menu();
}

void PVP_game_ONE(void)
{
	Board b = board_create();

	Piece p;

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 1);

	p = piece_create();
	piece_set_type(p, KING);
	piece_set_color(p, WHITE);
	board_link_piece(b, p, 7, 6);

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 6, 6);

	p = piece_create();
	piece_set_type(p, ROOK);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 6, 6);

	p = piece_create();
	piece_set_type(p, BISHOP);
	piece_set_color(p, BLACK);
	board_link_piece(b, p, 6, 4);

}

void debug_error_1(void)
{
	Board b = board_create();
	board_set_turn(b, WHITE);
	Piece p;

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 7, 0);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, KING);
	board_link_piece(b, p, 7, 2);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 7, 7);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 0);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 2);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 6);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 5, 2);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 5, 6);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 4, 5);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, KNIGHT);
	board_link_piece(b, p, 2, 4);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, KNIGHT);
	board_link_piece(b, p, 4, 2);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 2, 0);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 2, 5);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 1);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, KING);
	board_link_piece(b, p, 1, 3);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 7);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 0, 6);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 0, 7);

	display_print_board(b);
	if(gl_mate(b, board_get_turn(b)))
		printf("MATE\n");
	getchar();

	gl_move_piece(b, 2, 4, 3, 2);
	board_flip_turn(b);
	display_print_board(b);
	if(gl_mate(b, board_get_turn(b)))
		printf("MATE\n");
	getchar();
}

void debug_error_1_1(void)
{
	Board b = board_create();
	board_set_turn(b, BLACK);

	Piece p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 1);

	display_print_board(b);

	gl_move_piece(b, 1, 1, 2, 2);
	display_print_board(b);
}

void debug_error_2(void)
{
	Board b = board_create();
	board_set_turn(b, BLACK);
	Piece p;

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 7, 0);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, KING);
	board_link_piece(b, p, 7, 3);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, QUEEN);
	board_link_piece(b, p, 7, 4);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 7, 7);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 0);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 1);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 2);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, BISHOP);
	board_link_piece(b, p, 6, 3);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 6, 7);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, KNIGHT);
	board_link_piece(b, p, 5, 2);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 5, 3);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, BISHOP);
	board_link_piece(b, p, 4, 0);

	p = piece_create();
	piece_set_color(p, WHITE);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 4, 4);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, QUEEN);
	board_link_piece(b, p, 3, 4);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 2, 1);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, KNIGHT);
	board_link_piece(b, p, 2, 5);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 0);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, BISHOP);
	board_link_piece(b, p, 1, 1);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 2);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 3);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 6);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, PAWN);
	board_link_piece(b, p, 1, 7);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 0, 0);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, KING);
	board_link_piece(b, p, 0, 3);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, BISHOP);
	board_link_piece(b, p, 0, 5);

	p = piece_create();
	piece_set_color(p, BLACK);
	piece_set_type(p, ROOK);
	board_link_piece(b, p, 0, 7);

	display_print_board(b);
	getchar();

	gl_move_piece(b, 0, 3, 0, 1);
	display_print_board(b);
	gl_check(b, board_get_turn(b));
}
