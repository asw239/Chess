#include <stdio.h>
#include "../Board.h"

int main(int argc, char *argv[]){
	Board tst_brd = board_create();
	board_destroy(tst_brd);

	return 0;
}
