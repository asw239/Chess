#include "display_cli.h"
#include "../errors/def_hndl.h"
#include <wchar.h>
#include <locale.h>
#include <stdio.h>

#define FILE_NAME "display_cli.c"
#define QUADRANT_SIZE 3
#define OFFSET 9		// multiples of 3

static void padding_vert(void);
static void padding_hori(void);
static void div_hori(void);
static void letters(void);
static void div_vert(uint_fast8_t row, const Board b);
static void print_piece(const Piece p);

static void (*err_fnc_arr[ERROR_CODE_COUNT])(enum ErrorCode err,
	const char *msg) = {[GLOBAL_ERROR] = def_hndl};

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

enum APieceType{
	a_w_king	= 'K',
	a_w_queen 	= 'Q',
	a_w_rook 	= 'R',
	a_w_bishop 	= 'B',
	a_w_knight 	= 'N',
	a_w_pawn 	= 'P',

	a_b_king 	= 'k',
	a_b_queen	= 'q',
	a_b_rook	= 'r',
	a_b_bishop	= 'b',
	a_b_knight	= 'n',
	a_b_pawn	= 'p'
};

void init_display(void)
{
	setlocale(LC_ALL, "");
	//sets the locale to the locale of the execution environment
}

void print_board(const Board b)
{
	#define FUNC_NAME "void print_board(const Board b)"

	if(!b){
		if(!err_fnc_arr[NULL_PARAM])
			err_fnc_arr[GLOBAL_ERROR](NULL_PARAM, "In file "
				FILE_NAME ", " FUNC_NAME);
		else
			err_fnc_arr[NULL_PARAM](NULL_PARAM, " In file "
				FILE_NAME ", " FUNC_NAME);
	}

	#undef FUNC_NAME

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

static void div_vert(uint_fast8_t row, const Board b)
{
	for(uint_fast8_t i = 0; i < OFFSET; i++)
		printf(" ");

	printf("%d", row);
	printf(" |");
	for(uint_fast8_t i = 0; i < board_get_size(); i++){
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; j++)
			printf(" ");
		print_piece(board_get_piece(b, row - 1, i));
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; j++)
			printf(" ");

		printf("|");
	}

	printf(" %d", row);
}

static void print_piece(const Piece p)
{
	if(p == NULL)
		printf(" ");

	else if(piece_get_type(p) == KING && piece_get_color(p) == WHITE)
		printf("%lc", a_w_king);
	else if(piece_get_type(p) == KING && piece_get_color(p) == BLACK)
		printf("%lc", a_b_king);

	else if(piece_get_type(p) == QUEEN && piece_get_color(p) == WHITE)
		printf("%lc", a_w_queen);
	else if(piece_get_type(p) == QUEEN && piece_get_color(p) == BLACK)
		printf("%lc", a_b_queen);

	else if(piece_get_type(p) == ROOK && piece_get_color(p) == WHITE)
		printf("%lc", a_w_rook);
	else if(piece_get_type(p) == ROOK && piece_get_color(p) == BLACK)
		printf("%lc", a_b_rook);

	else if(piece_get_type(p) == BISHOP && piece_get_color(p) == WHITE)
		printf("%lc", a_w_bishop);
	else if(piece_get_type(p) == BISHOP && piece_get_color(p) == BLACK)
		printf("%lc", a_b_bishop);

	else if(piece_get_type(p) == KNIGHT && piece_get_color(p) == WHITE)
		printf("%lc", a_w_knight);
	else if(piece_get_type(p) == KNIGHT && piece_get_color(p) == BLACK)
		printf("%lc", a_b_knight);

	else if(piece_get_type(p) == PAWN && piece_get_color(p) == WHITE)
		printf("%lc", a_w_pawn);
	else if(piece_get_type(p) == PAWN && piece_get_color(p) == BLACK)
		printf("%lc", a_b_pawn);
}

void display_set_err_hndl(enum ErrorCode error_type,
	void (*err_hndl)(enum ErrorCode err, const char *msg))
{
	#define FUNC_NAME "void display_set_err_hndl(enum ErrorCode error_type"\
		", void (*err_hndl)(enum ErrorCode err, const char *msg))"

	if(!(
		error_type != GLOBAL_ERROR
		||
		error_type != NULL_PARAM
	))
		err_fnc_arr[GLOBAL_ERROR](INVALID_ENUM_PARAM, "In file "
			FILE_NAME ", " FUNC_NAME);

	#undef FUNC_NAME

	err_fnc_arr[error_type] = err_hndl;
}
