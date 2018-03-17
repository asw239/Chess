#ifndef DEF_HNDL_H
#define DEF_HDNL_H

#include "def_hndl.h"
#include <stdio.h>
#include <stdlib.h>

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
	default:
		fprintf(stderr, "ERROR #%d (UNKNOWN_ERR_NUM)\n", err);
	}
	exit(err);
}

#endif
