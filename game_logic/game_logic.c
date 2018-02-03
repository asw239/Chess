#include "game_logic.h"

Board generate_start_board()
{
	Board b = board_create();
	board_set_turn(b, 1);

	Piece **board_arr = board_get_arr(b);
	Piece tmp; 

	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 0);
	board_arr[0][0] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 7);
	board_arr[0][7] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 0);
	board_arr[7][0] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, rook);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 7);
	board_arr[7][7] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 1);
	board_arr[0][1] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 6);
	board_arr[0][6] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 1);
	board_arr[7][1] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, knight);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 6);
	board_arr[7][6] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 2);
	board_arr[0][2] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 5);
	board_arr[0][5] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 2);
	board_arr[7][2] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, bishop);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 5);
	board_arr[7][5] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 3);
	board_arr[0][3] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, queen);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 3);
	board_arr[7][3] = tmp;

	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, 0);
	piece_set_pos(tmp, 0, 4);
	board_arr[0][4] = tmp;
	tmp = piece_create();
	piece_set_type(tmp, king);
	piece_set_color(tmp, 1);
	piece_set_pos(tmp, 7, 4);
	board_arr[7][4] = tmp;

	for(int i = 1; i < board_get_size(); j+=5){
		for(int j = 0; j < board_get_size(); j++){
			tmp = piece_create();
			piece_set_type(tmp, pawn);
			i == 1?piece_set_color(tmp, 0):piece_set_color(tmp, 1);
			piece_set_pos(tmp, i, j);
			board_arr[i][j] = tmp;
		}
	}

	return b;
}
