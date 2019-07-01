#include <utility>
#include <cstdlib>
#include <ctime>
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

void AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void AI_player::init_player(int my_color, int op_color){
	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_board();

	if(my_color == BLACK) this->board[BOARD_SIZE/2][BOARD_SIZE/2] = BLACK; // refer to Simulator.cpp

	return;
}

int AI_player::return_my_color(){
	return this->my_color;
}

int AI_player::return_op_color(){
	return this->op_color;
}

void AI_player::set_forced_move(int color, int x, int y){
	this->board[x][y] = color;
}

pair< pair<int, int>, pair<int, int> > AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){
	pair< pair<int, int>, pair<int, int> > result;
	int x1, y1, x2, y2;

	// save opponent's move
	this->board[ last_op_move.first.first ][ last_op_move.first.second ] = this->board[ last_op_move.second.first ][ last_op_move.second.second ] = this->op_color;	

	while(true){
		x1 = rand()%BOARD_SIZE;
		y1 = rand()%BOARD_SIZE;

		if(this->board[x1][y1] == EMPTY) break;
	}

	while(true){
		x2 = rand()%BOARD_SIZE;
		y2 = rand()%BOARD_SIZE;

		if(this->board[x2][y2] == EMPTY) break;
	}

	// save player's move
	this->board[x1][y1] = this->board[x2][y2] = this->my_color;

	result.first.first = x1;
	result.first.second =  y1;
	result.second.first = x2;
	result.second.second =  y2;

	this->turn++;

	return result;
}