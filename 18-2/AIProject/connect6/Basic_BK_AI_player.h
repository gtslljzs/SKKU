#include <utility>
#include <tuple>
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

#ifndef BASIC_BK_AI_PLAYER_H
#define BASIC_BK_AI_PLAYER_H

class Basic_BK_AI_player : public AI_player{
	protected:
		int cand_size_1; // cand for first move
		int cand_size_2; // cand for second move per each first move
		float discount_factor;
		float my_weight[2][7];
		float op_weight[7];
		int my_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
		int op_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
		void init_board();
		void init_live_line_count();
		void set_move_update_live_line(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7], int color, int x, int y);	// should use this function for any move
		tuple<int,int,int> best_cand_for_move(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7]);
		int future_reward(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7]);
		pair< pair<int, int>, pair<int, int> > last_my_move;
		pair< pair<int, int>, pair<int, int> > return_whole_move(pair< pair<int, int>, pair<int, int> > last_op_move);
		pair< pair<int, int>, pair<int, int> > return_fixed_start_move();

	public:
		Basic_BK_AI_player();
		Basic_BK_AI_player(int cand_size_1, int cand_size_2, float discount_factor);
		void init_player(int my_color, int op_color);
		void set_forced_move(int color, int x, int y);
		pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
};

#endif