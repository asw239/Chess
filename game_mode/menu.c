#include "menu.h"
#include "../display/display_cli.h"
#include "player_vs_player.h"

void chess_menu(void)
{
	for(;;){
		clear_screen();
		print_menu(80, 25);
		char option = capture_menu();

		switch(option){
		case 'p':
			pvp_game_loop();
			continue;
		case 'e':
			continue;
		case 'a':
			continue;
		case 'x':
			return ;
		case '\0':
			continue;
		}
	}
}
