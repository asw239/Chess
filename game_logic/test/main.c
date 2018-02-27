#include "../game_logic.h"

int main(int argc, char *argv[])
{
	Board b = generate_start_board();
	board_destroy(&b);

	return 0;
}
