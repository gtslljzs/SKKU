#include <iostream>
#include <utility>
#include <ctime>
#include <cstdlib>
#include "Simulator.h"
#include "AI_player.h"
#include "LK_AI_player.h"
#include "BK_AI_player.h"
#include "Basic_AI_player.h"
#include "Basic_v1_AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#endif

using namespace std;

int main(){

	Simulator* main_simulator = new Simulator();
	AI_player* player1 = new BK_AI_player();
	AI_player* player2 = new LK_AI_player();

	srand((unsigned int)time(NULL));

	pair<int, int> result;

	result = main_simulator->start_simul(1, player1, player2, false);

	cout << "BLACK's win(O) : " << result.first << endl;
	cout << "WHITE's win(X) : " << result.second << endl;

	return 0;
}
