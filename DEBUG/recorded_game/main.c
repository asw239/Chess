#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include "../../game_mode/player_vs_player.h"

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("ERROR: no file specified as argument\n");
		return 1;
	}

	size_t cmd_sz = 128;
	char cmd[cmd_sz];
	memset(cmd, 0, cmd_sz);
	strncpy(cmd,
		".pgn-extract/pgn-extract ",
		cmd_sz);
	strncpy(cmd + strlen(cmd),
		"-Wuci --nomovenumbers --noresults --notags ",
		cmd_sz - strlen(cmd));
	strncpy(cmd + strlen(cmd),
		argv[1],
		cmd_sz - strlen(cmd));

	const size_t move_sz = 5;
	char move[move_sz];
	memset(move, 0, move_sz);
	//popen opens the command for input
	FILE *fp = popen(cmd, "r");
	fflush(stdout);
	pvp_game_loop(fp);

	pclose(fp);
	return 0;
}
