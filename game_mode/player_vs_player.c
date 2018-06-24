#include <stdio.h>
#include <stdlib.h>
#include "player_vs_player.h"
#include "../game_logic/game_logic.h"
#include "../display/display_cli.h"

static void set_error_callbacks(void);
static void unset_error_callbacks(void);
static void fnc_BOARD_EMPTY_SQUARE(enum ErrorCode err, const char *msg);
static void fnc_PIECE_MOVE_UNEXPECTED_COLOR(enum ErrorCode err,
	const char *msg);
static void fnc_PIECE_MOVE_ILLEGAL_CASTLE(enum ErrorCode err, const char *msg);
static void fnc_BOARD_INVALID_EN_PASSANT_PIECE(enum ErrorCode err,
	const char *msg);
static void fnc_PIECE_MOVE_OVERLAPS_ALLY(enum ErrorCode err, const char *msg);
static void fnc_PIECE_MOVE_NOT_IN_RANGE(enum ErrorCode err, const char *msg);
static void fnc_PIECE_MOVE_COLLISION(enum ErrorCode err, const char *msg);
static void fnc_PIECE_MOVE_KING_CHECKED(enum ErrorCode err, const char *msg);

void pvp_game_loop(FILE *s)
{
	Board b = gl_generate_start_board();
	set_error_callbacks();

	for(;;){
		display_clear_screen();
		display_print_board(b);

		if(gl_mate(b, board_get_turn(b))){
			display_victory();
			break;
		}

		uint_fast8_t r_old, c_old, r_new, c_new;
		if(!display_capture_move(s, &r_old, &c_old, &r_new, &c_new))
			continue;

		Board b_cpy = board_create_copy(b);

		if(!gl_move_piece(b, r_old, c_old, r_new, c_new)){
			board_destroy(&b_cpy);
			continue;
		}

		if(gl_check(b, board_get_turn(b))){
			fnc_PIECE_MOVE_KING_CHECKED(0, NULL);
			board_destroy(&b);
			b = b_cpy;
			continue;
		}

		board_destroy(&b_cpy);
		board_flip_turn(b);
	}

	unset_error_callbacks();
	board_destroy(&b);
}

static void set_error_callbacks(void)
{
	board_set_err_hndl(BOARD_EMPTY_SQUARE, fnc_BOARD_EMPTY_SQUARE);

	gl_set_err_hndl(PIECE_MOVE_UNEXPECTED_COLOR,
		fnc_PIECE_MOVE_UNEXPECTED_COLOR);
	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE,
		fnc_PIECE_MOVE_ILLEGAL_CASTLE);
	gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE,
		fnc_BOARD_INVALID_EN_PASSANT_PIECE);
	gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, fnc_PIECE_MOVE_OVERLAPS_ALLY);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, fnc_PIECE_MOVE_NOT_IN_RANGE);
	gl_set_err_hndl(PIECE_MOVE_COLLISION, fnc_PIECE_MOVE_COLLISION);
}

static void unset_error_callbacks(void)
{
	board_set_err_hndl(BOARD_EMPTY_SQUARE, NULL);


	gl_set_err_hndl(PIECE_MOVE_UNEXPECTED_COLOR, NULL);
	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, NULL);
	gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE, NULL);
	gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, NULL);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, NULL);
	gl_set_err_hndl(PIECE_MOVE_COLLISION, NULL);
}

static void fnc_BOARD_EMPTY_SQUARE(enum ErrorCode err, const char *msg)
{
	printf("ERROR: attempting to manipulate an empty square\n");
	display_pause();
}

static void fnc_PIECE_MOVE_UNEXPECTED_COLOR(enum ErrorCode err, const char *msg)
{
	printf("ERROR: attempting to move piece of opposing expected color\n");
	display_pause();
}

static void fnc_PIECE_MOVE_ILLEGAL_CASTLE(enum ErrorCode err, const char *msg)
{
	printf("ERROR: castling in the current board state is illegal\n");
	display_pause();
}

static void fnc_BOARD_INVALID_EN_PASSANT_PIECE(enum ErrorCode err,
	const char *msg)
{
	printf("ERROR: cannot en-passant capture on non-skip pawns\n");
	display_pause();
}

static void fnc_PIECE_MOVE_OVERLAPS_ALLY(enum ErrorCode err, const char *msg)
{
	printf("ERROR: attempting to move a Piece on top of an ally Piece\n");
	display_pause();
}

static void fnc_PIECE_MOVE_NOT_IN_RANGE(enum ErrorCode err, const char *msg)
{
	printf("ERROR: attempting to move a Piece outside of valid range\n");
	display_pause();
}

static void fnc_PIECE_MOVE_COLLISION(enum ErrorCode err, const char *msg)
{
	printf("ERROR: collision occured while moving piece\n");
	display_pause();
}

static void fnc_PIECE_MOVE_KING_CHECKED(enum ErrorCode err, const char *msg)
{
	printf("ERROR: move puts king in check\n");
	display_pause();
}
