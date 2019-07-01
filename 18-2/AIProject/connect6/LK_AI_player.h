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

#ifndef __LK_SETTING__
#define __LK_SETTING__
#define WINDOW 6
#define INF 1000
#endif

#ifndef LK_AI_PLAYER_H
#define LK_AI_PLAYER_H

class LK_AI_player : public AI_player{
	protected:
		int dx[8];
		int dy[8];
		int board_weight_defense[BOARD_SIZE][BOARD_SIZE];
		int board_weight_attack[BOARD_SIZE][BOARD_SIZE];
		double attack_penalty;
		double four_penalty;
		double future_penalty;
		int cand_size;
		vector<pair<int,int> > my_stone;
		vector<pair<int,int> > op_stone;
		void init_board();
		int check_four(int _type);
		void new_stone(int color, int x, int y);
		void modify_weight();
		vector<pair<int,pair<int,int> > > max_weight(int cand_size);
		pair<int,pair<int,int> > best_move(int num_move);

	public:
		LK_AI_player();
		LK_AI_player(double attack_penalty, double four_penalty, double future_penalty, int cand_size);
		void init_player(int my_color, int op_color);
		void set_forced_move(int color, int x, int y);
		pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
};

#endif