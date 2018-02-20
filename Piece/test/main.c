#include "../Piece.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

void errors(enum ErrorCode err, const char *msg);

int main(int argc, char **argv)
{
	Piece p = piece_create();

	piece_set_type(p, king);
	printf("piece type: ");
	switch(piece_get_type(p)){
	case 0:
		printf("king\n");
		break;
	case 1:
		printf("queen\n");
		break;
	case 2:
		printf("rook\n");
		break;
	case 3:
		printf("bishop\n");
		break;
	case 4:
		printf("knight\n");
		break;
	case 5:
		printf("pawn\n");
		break;
	}

	piece_set_color(p, WHITE);
	printf("piece color: ");
	piece_get_color(p) ? printf("white\n") : printf("black\n");

	piece_set_pos(p, 7, 7);
	printf("piece pos (");
	printf("%" PRIuFAST8, piece_get_x(p)); 
	printf(",%" PRIuFAST8, piece_get_y(p)); 
	printf(")\n");

	piece_destroy(&p);

	return 0;
}
