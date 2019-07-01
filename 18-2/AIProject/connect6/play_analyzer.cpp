#include <iostream>
#include <utility>
#include <ctime>
#include <cstdlib>
#include "Analyzer.h"
#include "AI_player.h"
#include "LK_AI_player.h"
#include "BK_AI_player.h"
#include "Basic_AI_player.h"
#include "Basic_v1_AI_player.h"
#include "Basic_BK_AI_player.h"
#include "UTT.h"

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

	Analyzer* main_analyzer = new Analyzer();
	AI_player* player1 = new LK_AI_player();
	AI_player* player2 = new BK_AI_player();

	srand((unsigned int)time(NULL));

	main_analyzer->start_game(player1, player2);

	return 0;
}