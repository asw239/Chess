#include "../Piece.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	Piece p = piece_create();

	enum PieceType pt = bishop;
	piece_set_type(p, pt);
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

	piece_destroy(p);
	return 0;
}
