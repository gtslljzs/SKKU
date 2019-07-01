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

#ifndef USER_AI_PLAYER_H
#define USER_AI_PLAYER_H

class USER_AI_player : public AI_player{
	protected:
		void init_board();
		void print_board();
	public:
		USER_AI_player();
		void init_player(int my_color, int op_color);
		void set_forced_move(int color, int x, int y);
		pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
};

#endif