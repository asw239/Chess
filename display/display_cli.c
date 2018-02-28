#include "display_cli.h"
#include <wchar.h>
#include <locale.h>
#include <stdio.h>

#define QUADRANT_SIZE 3
#define OFFSET 9		// multiples of 3

static void padding_vert(void);
static void padding_hori(void);
static void div_hori(void);
static void letters(void);
static void div_vert(uint_fast8_t row, Board b);
static void print_piece(Piece p);

enum UPieceType{
	u_w_king	= L'\u2654',
	u_w_queen 	= L'\u2655',
	u_w_rook 	= L'\u2656',
	u_w_bishop 	= L'\u2657',
	u_w_knight 	= L'\u2658',
	u_w_pawn 	= L'\u2659',

	u_b_king 	= L'\u265A',
	u_b_queen	= L'\u265B',
	u_b_rook	= L'\u265C',
	u_b_bishop	= L'\u265D',
	u_b_knight	= L'\u265E',
	u_b_pawn	= L'\u265F'
};

void init_display(void)
{
	setlocale(LC_ALL, "");
	//sets the locale to the locale of the execution environment
}

void print_board(Board b)
{
	padding_vert();
	letters();
	printf("\n");

	for(uint_fast8_t i = board_get_size(); i > 0; i--){
		div_hori();
		printf("\n");
		div_vert(i, b);
		printf("\n");
	}

	div_hori();
	printf("\n");
	letters();
	printf("\n");
	padding_vert();
}

static void padding_vert(void)
{
	for(uint_fast8_t i = 0; i < OFFSET / 3; i++)
		printf("\n");
}

static void padding_hori(void)
{
	for(uint_fast8_t i = 0; i < OFFSET; i++)
		printf(" ");
}

static void letters(void)
{
	padding_hori();
	printf("  ");

	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < (QUADRANT_SIZE - 1) / 2 + 1; j++)
			printf(" ");
		printf("%c", 'A' + i);
		for(uint_fast8_t j = 0; j < (QUADRANT_SIZE - 1) / 2; j++)
			printf(" ");
	}
}

static void div_hori(void)
{
	padding_hori();
	printf("  ");

	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		printf("+");
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE; j++)
			printf("-");
	}
	printf("+");
}

static void div_vert(uint_fast8_t row, Board b)
{
	for(uint_fast8_t i = 0; i < OFFSET; i++)
		printf(" ");

	printf("%d", row);
	printf(" |");
	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; j++)
			printf(" ");
		print_piece(board_get_arr(b)[row - 1][i]);
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; j++)
			printf(" ");

		printf("|");
	}

	printf(" %d", row);
}

static void print_piece(Piece p)
{
	if(p == NULL)
		printf(" ");

	else if(piece_get_type(p) == king && piece_get_color(p) == WHITE)
		printf("%lc", u_w_king);
	else if(piece_get_type(p) == king && piece_get_color(p) == BLACK)
		printf("%lc", u_b_king);

	else if(piece_get_type(p) == queen && piece_get_color(p) == WHITE)
		printf("%lc", u_w_queen);
	else if(piece_get_type(p) == queen && piece_get_color(p) == BLACK)
		printf("%lc", u_b_queen);

	else if(piece_get_type(p) == rook && piece_get_color(p) == WHITE)
		printf("%lc", u_w_rook);
	else if(piece_get_type(p) == rook && piece_get_color(p) == BLACK)
		printf("%lc", u_b_rook);

	else if(piece_get_type(p) == bishop && piece_get_color(p) == WHITE)
		printf("%lc", u_w_bishop);
	else if(piece_get_type(p) == bishop && piece_get_color(p) == BLACK)
		printf("%lc", u_b_bishop);

	else if(piece_get_type(p) == knight && piece_get_color(p) == WHITE)
		printf("%lc", u_w_knight);
	else if(piece_get_type(p) == knight && piece_get_color(p) == BLACK)
		printf("%lc", u_b_knight);

	else if(piece_get_type(p) == pawn && piece_get_color(p) == WHITE)
		printf("%lc", u_w_pawn);
	else if(piece_get_type(p) == pawn && piece_get_color(p) == BLACK)
		printf("%lc", u_b_pawn);
}
