#include <stdio.h>
#include "../game_logic.h"
#include "../../display/display_cli.h"

int main(int argc, char *argv[])
{
	Board tmp = generate_start_board();

	init_display();
	print_board(tmp);

	return 0;
}
