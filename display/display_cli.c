#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "display_cli.h"
#include "../errors/def_hndl.h"

#define QUADRANT_SIZE 3
#define OFFSET 6		// multiples of 3
#define CAPTURE_LIST_PADDING 1

#define FLUSH_EXCESS_INPUT() {\
	for(char c = getchar(); c != '\n' && c != EOF; c = getchar()) ;\
}

static void padding_vert(void);
static void padding_hori(void);
static void div_hori(void);
static void letters(void);
static void div_vert(uint_fast8_t row, const Board b);
static void print_piece(const Piece p);
static void print_capture_list(const Board b, enum PieceColor c);
static void print_turn(const Board b);
static void print_menu_board_top_bottom(uint_fast8_t menu_width);
static void print_menu_board_left_right(uint_fast8_t menu_width, char *msg);
void pause(void);

static ErrFncPtr err_fnc_arr[ERROR_CODE_COUNT] = {[GLOBAL_ERROR] =
	errors_def_hndl};
static const char *FILE_NAME = "display_cli.c";

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

void display_clear_screen(void)
{
	for(uint_fast16_t i = 0; i < 300; ++i)
		putchar('\n');
}

void display_print_board(const Board b)
{
	static const char *FUNC_NAME =
"void display_print_board(const Board b)";

	if(!b){
		call_error(err_fnc_arr, NULL_PARAM, FILE_NAME, FUNC_NAME);
		return ;
	}

	if(board_get_capture_list(b, BLACK)){
		print_capture_list(b, BLACK);
		printf("\n");
	}

	padding_vert();
	letters();
	printf("\n");

	for(uint_fast8_t i = BOARD_SIZE; i > 0; i--){
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

	if(board_get_capture_list(b, WHITE)){
		print_capture_list(b, WHITE);
		printf("\n");
	}

	print_turn(b);
}

static void padding_vert(void)
{
	for(uint_fast8_t i = 0; i < OFFSET / 3; ++i)
		printf("\n");
}

static void padding_hori(void)
{
	for(uint_fast8_t i = 0; i < OFFSET; ++i)
		printf(" ");
}

static void letters(void)
{
	padding_hori();
	printf("  ");

	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < (QUADRANT_SIZE - 1) / 2 + 1; ++j)
			printf(" ");
		printf("%c", 'A' + i);
		for(uint_fast8_t j = 0; j < (QUADRANT_SIZE - 1) / 2; ++j)
			printf(" ");
	}
}

static void div_hori(void)
{
	padding_hori();
	printf("  ");

	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		printf("+");
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE; ++j)
			printf("-");
	}
	printf("+");
}

static void div_vert(uint_fast8_t row, const Board b)
{
	for(uint_fast8_t i = 0; i < OFFSET; ++i)
		printf(" ");

	printf("%d", row);
	printf(" |");
	for(uint_fast8_t i = 0; i < BOARD_SIZE; ++i){
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; ++j)
			printf(" ");
		print_piece(board_get_piece(b, row - 1, i));
		for(uint_fast8_t j = 0; j < QUADRANT_SIZE / 2; ++j)
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

static void print_capture_list(const Board b, enum PieceColor c)
{
	padding_hori();
	uint_fast8_t printed_board_size =
		(QUADRANT_SIZE + 1) * BOARD_SIZE + 5;
	uint_fast8_t capture_list_size =
		PIECE_COUNT +
		(PIECE_COUNT * CAPTURE_LIST_PADDING) - CAPTURE_LIST_PADDING;
	for(
		uint_fast8_t i = 0;
		i < (printed_board_size - capture_list_size) / 2;
		++i
	)
		printf(" ");

	for(uint_fast8_t i = 0; i < PIECE_COUNT; ++i){
		for(uint_fast8_t j = 0; j < CAPTURE_LIST_PADDING; ++j){
			if(i != 0)
				printf(" ");
			if(c == WHITE)
				print_piece(
					board_get_capture_list(b, WHITE)[i]
				);
			else
				print_piece(
					board_get_capture_list(b, BLACK)[i]
				);
		}
	}
}

static void print_turn(const Board b)
{
	putchar('\n');
	padding_hori();

	uint_fast8_t printed_board_size =
		(QUADRANT_SIZE + 1) * BOARD_SIZE + 5;
	const char *w_msg = "*WHITE'S TURN*";
	const char *b_msg = "*BLACK'S TURN*";
	uint_fast8_t msg_length = 14;

	for(uint_fast8_t i = 0; i < (printed_board_size - msg_length) / 2; ++i)
		putchar(' ');

	board_get_turn(b) == WHITE ?
		printf("%s\n", w_msg) :
		printf("%s\n", b_msg);
}

void display_print_menu(uint_fast8_t menu_width, uint_fast8_t menu_height)
{
	static const char *FUNC_NAME =
"void display_print_menu(uint_fast8_t menu_width, uint_fast8_t menu_height)";


	char *pvp = "(P) Player Vs. Player (P)";
	char *pve = "(E) Player Vs. A.I. (E)";
	char *eve = "(A) A.I. Vs. A.I. (A)";
	char *exit = "(X) Exit (X)";
	uint_fast8_t option_count = 4 + 1;
	uint_fast8_t grtst_msg_lgth = strlen(pvp);

	if(menu_width < grtst_msg_lgth + 2){
		call_error(err_fnc_arr, DISPLAY_WIDTH_SMALL, FILE_NAME,
			FUNC_NAME);
		return ;
	}
	if(menu_height < option_count + 2){
		call_error(err_fnc_arr, DISPLAY_HEIGHT_SMALL, FILE_NAME,
			FUNC_NAME);
		return ;
	}

	print_menu_board_top_bottom(menu_width);

	uint_fast8_t space_height = (menu_height - 2 - option_count) / 2 ;
	for(uint_fast8_t i = 0; i < space_height; ++i)
		print_menu_board_left_right(menu_width, NULL);

	print_menu_board_left_right(menu_width, NULL);
	print_menu_board_left_right(menu_width, NULL);
	print_menu_board_left_right(menu_width, NULL);

	print_menu_board_left_right(menu_width, pvp);
	print_menu_board_left_right(menu_width, pve);
	print_menu_board_left_right(menu_width, eve);
	print_menu_board_left_right(menu_width, NULL);
	print_menu_board_left_right(menu_width, exit);

	for(uint_fast8_t i = 0; i < space_height; ++i)
		print_menu_board_left_right(menu_width, NULL);

	print_menu_board_top_bottom(menu_width);
}

static void print_menu_board_top_bottom(uint_fast8_t menu_width)
{
	for(uint_fast8_t i = 0; i < menu_width; ++i)
		putchar('*');
	putchar('\n');
}

static void print_menu_board_left_right(uint_fast8_t menu_width, char *msg)
{
	if(!msg){
		putchar('*');
		for(uint_fast8_t i = 0; i < menu_width - 2; ++i)
			putchar(' ');
		printf("*\n");

	}else{
		uint_fast8_t msg_length = strlen(msg);
		uint_fast8_t space_length = (menu_width - 2 - msg_length) / 2 ;
		uint_fast8_t offset =
			((menu_width - 2 - msg_length) % 2) ? 1 : 0;

		putchar('*');
		for(uint_fast8_t i = 0; i < space_length; ++i)
			putchar(' ');
		printf("%s", msg);
		for(uint_fast8_t i = 0; i < space_length + offset; ++i)
			putchar(' ');
		printf("*\n");
	}
}

char display_capture_menu(void)
{
	static const char *FUNC_NAME =
"char display_capture_menu(void)";

	errors_terminate_on_def_err = false;

	printf("\nEnter menu selection: ");

	char option;
	if(scanf(" %c", &option) != 1){
		FLUSH_EXCESS_INPUT();
		call_error(err_fnc_arr, CAPTURE_MENU_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return '\0';
	}
	FLUSH_EXCESS_INPUT();

	switch(option){
	case 'P':
	case 'E':
	case 'A':
	case 'X':
		errors_terminate_on_def_err = true;
		return toupper(option);
	case 'p':
	case 'e':
	case 'a':
	case 'x':
		errors_terminate_on_def_err = true;
		return option;
	default:
		FLUSH_EXCESS_INPUT();
		call_error(err_fnc_arr, CAPTURE_MENU_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return '\0';
	}
}

void display_pause(void)
{
	printf("\nPress any key to continue: ");
	FLUSH_EXCESS_INPUT();
}

bool display_capture_move(uint_fast8_t *r_old, uint_fast8_t *c_old,
	uint_fast8_t *r_new, uint_fast8_t *c_new)
{
	static const char *FUNC_NAME =
"bool display_capture_move(uint_fast8_t *r_old, uint_fast8_t *c_old, \
uint_fast8_t *r_new, uint_fast8_t *c_new)";

	errors_terminate_on_def_err = false;

	printf("\nEnter source destination: ");

	char cr_old, cc_old, space, cr_new, cc_new;
	if(scanf(" %c%c%c%c%c", &cr_old,&cc_old,&space,&cr_new,&cc_new) != 5){
		FLUSH_EXCESS_INPUT();
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}
	FLUSH_EXCESS_INPUT();

	if(cr_old >= '1' && cr_old <= '8'){
		*r_old = cr_old - '1';
	}else{
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}

	if(cc_old >= 'a' && cc_old <= 'h'){
		*c_old = cc_old - 'a';
	}else if(cc_old >= 'A' && cc_old <= 'H'){
		*c_old = cc_old - 'A';
	}else{
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}

	if(space != ' '){
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}

	if(cr_new >= '1' && cr_new <= '8'){
		*r_new = cr_new - '1';
	}else{
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}

	if(cc_new >= 'a' && cc_new <= 'h'){
		*c_new = cc_new - 'a';
	}else if(cc_old >= 'A' && cc_old <= 'H'){
		*c_new = cc_new - 'A';
	}else{
		call_error(err_fnc_arr, CAPTURE_MOVE_BAD_INP, FILE_NAME,
			FUNC_NAME);
		pause();
		return false;
	}

	errors_terminate_on_def_err = true;
	return true;
}

ErrFncPtr display_set_err_hndl(enum ErrorCode error_type,
	ErrFncPtr err_hndl)
{
	static const char *FUNC_NAME =
"ErrFncPtr display_set_err_hndl(enum ErrorCode error_type, \
ErrFncPtr err_hndl)";

	if(!(
		error_type != GLOBAL_ERROR
		||
		error_type != NULL_PARAM
		||
		error_type != DISPLAY_WIDTH_SMALL
		||
		error_type != DISPLAY_HEIGHT_SMALL
		||
		error_type != CAPTURE_MENU_BAD_INP
		||
		error_type != CAPTURE_MOVE_BAD_INP
	)){
		call_error(err_fnc_arr, INVALID_ENUM_PARAM, FILE_NAME,
			FUNC_NAME);
		return NULL;
	}

	ErrFncPtr tmp = err_fnc_arr[error_type];
	err_fnc_arr[error_type] = err_hndl;
	return tmp;
}
