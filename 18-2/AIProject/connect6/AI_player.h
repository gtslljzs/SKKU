#include <utility>
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

#ifndef AI_PLAYER_H
#define AI_PLAYER_H

class AI_player{
	protected:
		int my_color;
		int op_color;
		int turn;
		int board[BOARD_SIZE][BOARD_SIZE];
		virtual void init_board();

	public:
		int return_my_color();
		int return_op_color();
		virtual void init_player(int my_color, int op_color);
		virtual void set_forced_move(int color, int x, int y);
		virtual pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
};

#endif