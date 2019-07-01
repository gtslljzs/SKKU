#include <utility>
#include "AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#endif

#ifndef SIMULATOR_H
#define SIMULATOR_H
#define MAX_TURN 180

class Simulator{
	protected:
		int board[BOARD_SIZE][BOARD_SIZE];
		/*
			same as project's rule

			(x,y) = board[ x ][ y ]
			(0,18) ... (18,18)
			 ...		   ...
			 (0,0) ...  (18,0)
		*/
		void init_board();
		void print_board();
		int get_max_connected_len(int player, int x, int y);
		int update_board(int player, pair< pair<int, int>, pair<int, int> > move, bool print_flag);
		int start_one_simul(AI_player* black_player, AI_player* white_player, bool print_flag);

	public:
		pair<int, int> start_simul(int num_simul, AI_player* black_player, AI_player* white_player, bool print_flag);

};

#endif