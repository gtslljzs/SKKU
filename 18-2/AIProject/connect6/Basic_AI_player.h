#include <utility>
#include "AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

#ifndef BASIC_AI_PLAYER_H
#define BASIC_AI_PLAYER_H

class Basic_AI_player : public AI_player{
	protected:
		float my_weight[7];
		float op_weight[7];
		int scored_board[BOARD_SIZE][BOARD_SIZE];
		int my_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
		int op_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
		void init_board();
		void init_live_line_count();
		void set_move_update_live_line(int color, int x, int y);	// should use this function for any move
		pair< pair<int, int>, pair<int, int> > last_my_move;
		pair<int, int> return_half_move(pair< pair<int, int>, pair<int, int> > last_op_move, int half_turn); // true for first, false for second
		pair< pair<int, int>, pair<int, int> > return_fixed_start_move();

	public:
		Basic_AI_player();
		Basic_AI_player(float my_0, float my_1, float my_2, float my_3, float my_4, float my_5, float my_6,
						float op_0, float op_1, float op_2, float op_3, float op_4, float op_5, float op_6);
		void init_player(int my_color, int op_color);
		void set_forced_move(int color, int x, int y);
		void debug_print_my_live_line_count(int ll);
		void debug_print_op_live_line_count(int ll);
		pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
};

#endif