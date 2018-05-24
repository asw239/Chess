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

static bool move_legal = true;
void pvp_game_loop(void)
{
	Board b = generate_start_board();
	set_error_callbacks();

	for(;;){
		if(mate(b, board_get_turn(b)))
			break;

		clear_screen();
		print_board(b);

		uint_fast8_t r_old, c_old, r_new, c_new;
		if(!capture_move(&r_old, &c_old, &r_new, &c_new))
			continue;

		Board b_cpy = board_create_copy(b);

		board_move_piece(b, r_old, c_old, r_new, c_new);
		if(!move_legal){
			move_legal = true;
			continue;
		}

		if(check(b, board_get_turn(b))){
			fnc_PIECE_MOVE_KING_CHECKED(0, NULL);
			board_destroy(&b);
			b = b_cpy;
			continue;
		}

		board_flip_turn(b);
	}

	unset_error_callbacks();
	board_destroy(&b);
}

static void set_error_callbacks(void)
{
	board_set_err_hndl(BOARD_EMPTY_SQUARE, fnc_BOARD_EMPTY_SQUARE);
	board_set_err_hndl(PIECE_MOVE_UNEXPECTED_COLOR,
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
	board_set_err_hndl(PIECE_MOVE_UNEXPECTED_COLOR, NULL);

	gl_set_err_hndl(PIECE_MOVE_ILLEGAL_CASTLE, NULL);
	gl_set_err_hndl(BOARD_INVALID_EN_PASSANT_PIECE, NULL);
	gl_set_err_hndl(PIECE_MOVE_OVERLAPS_ALLY, NULL);
	gl_set_err_hndl(PIECE_MOVE_NOT_IN_RANGE, NULL);
	gl_set_err_hndl(PIECE_MOVE_COLLISION, NULL);
}

static void fnc_BOARD_EMPTY_SQUARE(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: attempting to manipulate an empty square\n");
	pause();
}

static void fnc_PIECE_MOVE_UNEXPECTED_COLOR(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: attempting to move piece of opposing expected color\n");
	pause();
}

static void fnc_PIECE_MOVE_ILLEGAL_CASTLE(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: castling in the current board state is illegal\n");
	pause();
}

static void fnc_BOARD_INVALID_EN_PASSANT_PIECE(enum ErrorCode err,
	const char *msg)
{
	move_legal = false;
	printf("ERROR: cannot en-passant capture on non-skip pawns\n");
	pause();
}

static void fnc_PIECE_MOVE_OVERLAPS_ALLY(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: attempting to move a Piece on top of an ally Piece\n");
	pause();
}

static void fnc_PIECE_MOVE_NOT_IN_RANGE(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: attempting to move a Piece outside of valid range\n");
	pause();
}

static void fnc_PIECE_MOVE_COLLISION(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: collision occured while moving piece\n");
	pause();
}

static void fnc_PIECE_MOVE_KING_CHECKED(enum ErrorCode err, const char *msg)
{
	move_legal = false;
	printf("ERROR: move puts king in check\n");
	pause();
}