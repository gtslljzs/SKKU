#include <utility>
#include <vector>
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

#ifndef BK_PLAYER_H
#define BK_PLAYER_H

class BK_AI_player : public AI_player{
	protected:
		int active_board[BOARD_SIZE][BOARD_SIZE];
		struct Window{
			int type;
			int count = 0;
			int end_x;
			int end_y;
		};
		struct Stone{
			int first_x;
			int first_y;
			int second_x;
			int second_y;
			int score;
		};
		vector< Window > deactivate;
		vector< Window > cur_my;
		vector< Window > cur_op;
		struct Stone next_stone;
		void init_board();
		void init_active_board();
		void updateThreat( int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete, int _x, int _y );
		void firstSearch( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete );
		void secondSearch( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete );
		
	public:
		void init_player(int my_color, int op_color);
		void set_forced_move(int color, int x, int y);
		pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
		void printBoard( int _board[][BOARD_SIZE] );
};

#endif