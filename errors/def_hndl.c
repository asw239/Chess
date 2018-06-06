#include "def_hndl.h"
#include <stdio.h>
#include <stdlib.h>

bool terminate_on_def_err = true;

void def_hndl(enum ErrorCode err, const char *msg)
{
	fprintf(stderr, "\n%s\n", msg);

	switch(err){
	case MEM_FAIL:
		fprintf(stderr, "ERROR #%d (MEM_FAIL)\n", err);
		fprintf(stderr,
			"malloc returned NULL, program out of usable memory\n");
		break;
	case NULL_PARAM:
		fprintf(stderr, "ERROR #%d (NULL_PARAM)\n", err);
		fprintf(stderr,
			"unexpected NULL value as a parameter\n");
		break;
	case INVALID_ENUM_PARAM:
		fprintf(stderr, "ERROR #%d (INVALID_ENUM_PARAM)\n", err);
		fprintf(stderr,
			"enum parameter is not part of valid identifiers\n");
		break;
	case INVALID_INT_PARAM:
		fprintf(stderr, "ERROR #%d (INVALID_INT_PARAM)\n", err);
		fprintf(stderr,
			"int parameter is not part of a valid range\n");
		break;
	case PIECE_NOT_IN_BOARD:
		fprintf(stderr, "ERROR #%d (PIECE_NOT_IN_BOARD)\n", err);
		fprintf(stderr,
			"Piece parameter is not associated with that Board\n");
		break;
	case CONFLICTING_PARAM:
		fprintf(stderr, "ERROR #%d (CONFLICTING PARAM)\n", err);
		fprintf(stderr,
			"destroy must = false after board_init_capture_list\n");
		break;
	case PIECE_MOVE_SAME_POS:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_SAME_POS)\n", err);
		fprintf(stderr,
			"attempting to move a Piece to the same position\n");
		break;
	case PIECE_MOVE_OVERLAPS_ALLY:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_OVERLAPS_ALLY)\n", err);
		fprintf(stderr,
			"attempting to move a Piece on top of ally Piece\n");
		break;
	case PIECE_MOVE_NOT_IN_RANGE:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_NOT_IN_RANGE)\n", err);
		fprintf(stderr,
			"attempting to move a Piece outside of valid range\n");
		break;
	case PIECE_MOVE_COLLISION:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_COLLISION)\n", err);
		fprintf(stderr,
			"collision occured while moving piece\n");
		break;
	case PIECE_MOVE_KING_CHECKED:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_KING_CHECKED)\n", err);
		fprintf(stderr,
			"king is under check, move piece appropriately\n");
		break;
	case BOARD_EMPTY_SQUARE:
		fprintf(stderr, "ERROR #%d (BOARD_EMPTY_SQUARE)\n",
			err);
		fprintf(stderr,
			"attempting to manipulate an empty square\n");
		break;
	case BOARD_NONEMPTY_SQUARE:
		fprintf(stderr, "ERROR #%d (BOARD_NONEMPTY_SQUARE)\n"
			, err);
		fprintf(stderr,
			"attempting to overwrite a non-empty square\n");
		break;
	case PIECE_MOVE_ILLEGAL_CASTLE:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_ILLEGAL_CASTLE)\n", err);
		fprintf(stderr,
			"castling in the current board state is illegal\n");
		break;
	case PIECE_MOVE_UNEXPECTED_COLOR:
		fprintf(stderr, "ERROR #%d (PIECE_MOVE_UNEXPECTED_COLOR)\n",
			err);
		fprintf(stderr,
			"attempting to move piece of opposing expected color\n");
		break;
	case BOARD_INVALID_EN_PASSANT_PIECE:
		fprintf(stderr, "ERROR #%d (BOARD_INVALID_EN_PASSANT_PIECE)\n",
			err);
		fprintf(stderr,
			"cannot en-passant capture on non-skip pawns\n");
		break;
	case DISPLAY_WIDTH_SMALL:
		fprintf(stderr, "ERROR #%d (DISPLAY_WIDTH_SMALL)\n", err);
		fprintf(stderr,
			"width parameter too small for proper display\n");
		break;
	case DISPLAY_HEIGHT_SMALL:
		fprintf(stderr, "ERROR #%d (DISPLAY_HEIGHT_SMALL)\n", err);
		fprintf(stderr,
			"height parameter too small for proper display\n");
		break;
	case CAPTURE_MENU_BAD_INP:
		fprintf(stderr, "ERROR #%d (CAPTURE_MENU_BAD_INP)\n", err);
		fprintf(stderr,
			"input not part of menu options\n");
		break;
	case CAPTURE_MOVE_BAD_INP:
		fprintf(stderr, "ERROR #%d (CAPTURE_MOVE_BAD_INP)\n", err);
		fprintf(stderr,
			"invalid input format for capture_move\n");
		fprintf(stderr,
			"Expected format: [rc rc] where r 1-8 and c A-H\n");
		break;
	default:
		fprintf(stderr, "ERROR #%d (UNKNOWN_ERR_NUM)\n", err);
	}

	if(terminate_on_def_err)
		exit(err);
}
