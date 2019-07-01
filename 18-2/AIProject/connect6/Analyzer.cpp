#include <iostream>
#include <utility>
#include "Analyzer.h"
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

void Analyzer::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;
}

void Analyzer::print_board(){
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

int Analyzer::get_max_connected_len(int player, int x, int y){
	int max_len = 0, len;

	//horizontal
	len = 1;
	for(int i=-1 ; ( 0 <= x+i ) ; i--){
		if(this->board[x+i][y] == player) len++;
		else break;
	}
	for(int i=1 ; ( x+i < BOARD_SIZE ) ; i++){
		if(this->board[x+i][y] == player) len++;
		else break;	
	}

	if(len > max_len) max_len = len;

	//vertical
	len = 1;
	for(int i=-1 ; ( 0 <= y+i ) ; i--){
		if(this->board[x][y+i] == player) len++;
		else break;
	}
	for(int i=1 ; ( y+i < BOARD_SIZE ) ; i++){
		if(this->board[x][y+i] == player) len++;
		else break;	
	}

	if(len > max_len) max_len = len;

	//top_left_down_right diagonal
	len = 1;
	for(int i=-1 ; ( 0 <= x+i && y-i < BOARD_SIZE ) ; i--){
		if(this->board[x+i][y-i] == player) len++;
		else break;
	}
	for(int i=1 ; ( x+i < BOARD_SIZE && 0 <= y-i) ; i++){
		if(this->board[x+i][y-i] == player) len++;
		else break;	
	}

	if(len > max_len) max_len = len;

	//top_right_down_left diagonal	
	len = 1;
	for(int i=-1 ; ( 0 <= x+i && 0 <= y+i) ; i--){
		if(this->board[x+i][y+i] == player) len++;
		else break;
	}
	for(int i=1 ; ( x+i < BOARD_SIZE && y+i < BOARD_SIZE ) ; i++){
		if(this->board[x+i][y+i] == player) len++;
		else break;	
	}

	if(len > max_len) max_len = len;

	return max_len;
}

int Analyzer::update_board(int player, pair< pair<int, int>, pair<int, int> > move, bool print_flag){
	int x1 = move.first.first;
	int y1 = move.first.second;
	int x2 = move.second.first;
	int y2 = move.second.second;
	int max_connected_len1, max_connected_len2;

	if(!( 0<=x1 && x1<=BOARD_SIZE &&
			 0<=y1 && y1<=BOARD_SIZE &&
			 0<=x2 && x2<=BOARD_SIZE &&
			 0<=y2 && y2<=BOARD_SIZE )){
		
		if(print_flag == true){
				cout << ((player == BLACK) ? "BLACK" : "WHITE") << " moved out of range" << endl;
				cout << ((player == BLACK) ? "BLACK's" : "WHITE's") << " latest move: " << endl;
				cout << "	" << (char)('A' + x1) << " " << y1 << endl;
				cout << "	" << (char)('A' + x2) << " " << y2 << endl;
		}
		return -1; // out of range -> lose

	}else{
		if(this->board[x1][y1] == EMPTY && this->board[x2][y2] == EMPTY) this->board[x1][y1] = this->board[x2][y2] = player;
		else{
			cout << ((player == BLACK) ? "BLACK" : "WHITE") << " sent overllaped move" << endl;
			return -1;
		}

		max_connected_len1 = get_max_connected_len(player,x1,y1);
		max_connected_len2 = get_max_connected_len(player,x2,y2);

		if( max_connected_len1 > 6 || max_connected_len2 > 6 ){
			if(print_flag == true){
				cout << ((player == BLACK) ? "BLACK" : "WHITE") << " made line over 6" << endl;
				cout << ((player == BLACK) ? "BLACK's" : "WHITE's") << " latest move: " << endl;
				cout << "	" << (char)('A' + x1) << " " << y1 << endl;
				cout << "	" << (char)('A' + x2) << " " << y2 << endl;
			}
			return -1; // over 6 -> lose
		}
		else{
			if( max_connected_len1==6 || max_connected_len2==6 ){
				if(print_flag == true){
					cout << ((player == BLACK) ? "BLACK" : "WHITE") << " made line 6" << endl;
					cout << ((player == BLACK) ? "BLACK's" : "WHITE's") << " latest move: " << endl;
					cout << "	" << (char)('A' + x1) << " " << y1 << endl;
					cout << "	" << (char)('A' + x2) << " " << y2 << endl;
				}
				return 1; // 6 -> win
			}
			else return 0; // continue game
		}
	}
}

void Analyzer::start_game(AI_player* black_player, AI_player* white_player){// RETURN WINNER (-1 -> draw)
	int turn = 0, update_result;
	int start_x , start_y, current_my_color, current_op_color;
	char buf[100];
	AI_player* current_player;
	start_x = start_y = BOARD_SIZE/2;

	init_board();

	black_player->init_player(BLACK, WHITE);
	white_player->init_player(WHITE, BLACK);

	cout << "Start Game! (input any keys)" << endl;
	cin >> buf;

	// start with centered black's move
	pair< pair<int, int>, pair<int, int> > last_op_move = make_pair( make_pair(start_x,start_y),
																	make_pair(start_x,start_y) );
	update_board(BLACK, last_op_move, true);

	black_player->set_forced_move(BLACK, start_x, start_y);
	white_player->set_forced_move(BLACK, start_x, start_y);

	cout << "================================== " << "Black moved" << " ==================================" << endl;
	print_board();
	cout << "\n(input any keys)" << endl;
	cin >> buf;

	while(turn < MAX_TURN){

		current_player = ((turn%2 == 0) ? white_player : black_player);
		current_my_color = current_player->return_my_color();
		current_op_color = current_player->return_op_color();

		last_op_move = current_player->return_move(last_op_move);
		update_result = update_board(current_my_color, last_op_move, true);
		if(update_result == 1 || update_result == -1) break;

		if((turn%2) == 0) cout << "================================== " <<  "White moved" << " ==================================" << endl;
		else cout << "================================== " << "Black moved" << " ==================================" << endl;
		print_board();
		cout << "\n(input any keys)" << endl;
		cin >> buf;

		turn++;
	}

	if(turn == MAX_TURN){
		cout << "================================== " << "Game Over" << " ==================================" << endl;
		cout << "Draw" << endl;
	}
	print_board();

	return;
}