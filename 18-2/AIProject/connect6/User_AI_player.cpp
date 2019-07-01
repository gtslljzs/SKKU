#include <utility>
#include <cstdlib>
#include <iostream>
#include "AI_player.h"
#include "USER_AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

USER_AI_player::USER_AI_player(){
}

void USER_AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void USER_AI_player::print_board(){
	cout << endl;
	for(int j = BOARD_SIZE - 1; j >= 0; j--){
		cout << j << "	";
		for(int i = 0; i<BOARD_SIZE; i++){
			if(this->board[i][j] == EMPTY) cout << "- " ;
			else if(this->board[i][j] == BLACK) cout << "O ";
			else if(this->board[i][j] == WHITE) cout << "X ";
		}
		cout << endl;
	}
	cout << "\n	A B C D E F G H I J K L M N O P Q R S " << endl;

	return;
}

void USER_AI_player::init_player(int my_color, int op_color){
	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_board();

	return;
}

void USER_AI_player::set_forced_move(int color, int x, int y){
	this->board[x][y] = color;

	return;
}

pair< pair<int, int>, pair<int, int> > USER_AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){
	char c1, c2;
	int i1, i2;
	pair< pair<int, int>, pair<int, int> > result;

	// save opponent's move
	if(!(this->my_color == WHITE && this->turn == 0)){
		this->board[last_op_move.first.first][last_op_move.first.second] = this->board[last_op_move.second.first][last_op_move.second.second] = this->op_color;
	}

	if(this->my_color == WHITE) cout << "=========================================== Your Turn (White, X) ===========================================" << endl;
	else cout << "=========================================== Your Turn (BLACK, O) ===========================================" << endl;
	cout << "OP's latest move: " << (char)('A' + last_op_move.first.first) << last_op_move.first.second << ", " << (char)('A' + last_op_move.second.first) << last_op_move.second.second << endl;
	print_board();
	while(true){
		cout << "Move 1: ";
		cin >> c1 >> i1;	
		if( ('A' <= c1 && c1 <= 'S') && (0 <= i1 && i1 <= BOARD_SIZE - 1) ) break;
	}
	while(true){
		cout << "Move 2: ";
		cin >> c2 >> i2;	
		if( ('A' <= c2 && c2 <= 'S') && (0 <= i2 && i2 <= BOARD_SIZE - 1) ) break;
	}

	result.first.first = (int)(c1-'A');
	result.first.second = i1;
	result.second.first = (int)(c2-'A');
	result.second.second = i2;
	
	this->board[result.first.first][result.first.second] = this->board[result.second.first][result.second.second] = this->my_color;

	// save player's move
	this->turn++;
	return result;
}