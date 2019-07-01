#include <iostream>
#include <utility>
#include "Simulator.h"
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

void Simulator::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;
}

void Simulator::print_board(){
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

int Simulator::get_max_connected_len(int player, int x, int y){
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

int Simulator::update_board(int player, pair< pair<int, int>, pair<int, int> > move, bool print_flag){
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

int Simulator::start_one_simul(AI_player* black_player, AI_player* white_player, bool print_flag){// RETURN WINNER (-1 -> draw)
	int turn = 0, update_result, simul_result = -1;
	int start_x , start_y, current_my_color, current_op_color;
	AI_player* current_player;
	start_x = start_y = BOARD_SIZE/2;

	init_board();

	black_player->init_player(BLACK, WHITE);
	white_player->init_player(WHITE, BLACK);

	// start with centered black's move
	pair< pair<int, int>, pair<int, int> > last_op_move = make_pair( make_pair(start_x,start_y),
																	make_pair(start_x,start_y) );
	update_board(BLACK, last_op_move, print_flag);

	black_player->set_forced_move(BLACK, start_x, start_y);
	white_player->set_forced_move(BLACK, start_x, start_y);

	while(turn < MAX_TURN){

		current_player = ((turn%2 == 0) ? white_player : black_player);
		current_my_color = current_player->return_my_color();
		current_op_color = current_player->return_op_color();

		last_op_move = current_player->return_move(last_op_move);
		update_result = update_board(current_my_color, last_op_move, print_flag);
		if(update_result == 1){
			simul_result = current_my_color;
			break;
		}else if(update_result == -1){
			simul_result = current_op_color;
			break;
		}

		turn++;
	}

	if(print_flag == true){
		if(simul_result == -1) cout << "Draw" << endl;
		print_board();
	}
	return simul_result;
}

pair<int, int> Simulator::start_simul(int num_simul, AI_player* black_player, AI_player* white_player, bool print_flag){
	int result;
	pair<int, int> result_count(0,0);

	for(int epoch = 0; epoch < num_simul; epoch++){
		if(print_flag == true) cout << "\n==============================\n" << endl;
		result = start_one_simul(black_player, white_player, print_flag);
		if(print_flag == true) cout << "\n==============================\n" << endl;

		if(result == BLACK) result_count.first++;
		else if(result == WHITE) result_count.second++;
	}

	return result_count;
}