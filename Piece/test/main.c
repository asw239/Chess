#include "../Piece.h"

int main(int argc, char **argv)
{
	Piece p = piece_create();
	piece_destroy(p);

	return 0;
}
