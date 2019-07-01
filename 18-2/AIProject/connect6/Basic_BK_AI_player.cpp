#include <utility>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "AI_player.h"
#include "Basic_BK_AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

bool _compare(tuple<int,int,int,int,int> a, tuple<int,int,int,int,int> b){
	return get<0>(a) > get<0>(b);
}

Basic_BK_AI_player::Basic_BK_AI_player(){

	// weight for first half move
	this->my_weight[0][0] = 0;
	this->my_weight[0][1] = 10000;
	this->my_weight[0][2] = 20000;
	this->my_weight[0][3] = 30000; //fix
	this->my_weight[0][4] = 27000000; //fix
	this->my_weight[0][5] = 27000000; //fix
	this->my_weight[0][6] = 0;

	// weight for second half move
	this->my_weight[1][0] = 0;
	this->my_weight[1][1] = 10000;
	this->my_weight[1][2] = 20000;
	this->my_weight[1][3] = 30000;
	this->my_weight[1][4] = 30000; // second turn's 4 is not important (making new move 4 is more valuable than making move 4 to move 5)
	this->my_weight[1][5] = 27000000; //fix
	this->my_weight[1][6] = 0;

	this->op_weight[0] = 0;
	this->op_weight[1] = 10000;
	this->op_weight[2] = 20000;
	this->op_weight[3] = 30000;
	this->op_weight[4] = 900000; // fix
	this->op_weight[5] = 900000; // fix
	this->op_weight[6] = 0;

	this->cand_size_1 = 10;
	this->cand_size_2 = 10;
	this->discount_factor = 1;
}

Basic_BK_AI_player::Basic_BK_AI_player(int cand_size_1, int cand_size_2, float discount_factor){

	// weight for first half move
	this->my_weight[0][0] = 100;
	this->my_weight[0][1] = 10000;
	this->my_weight[0][2] = 20000;
	this->my_weight[0][3] = 30000; //fix
	this->my_weight[0][4] = 27000000; //fix
	this->my_weight[0][5] = 27000000; //fix
	this->my_weight[0][6] = 27000000;

	// weight for second half move
	this->my_weight[1][0] = 100;
	this->my_weight[1][1] = 10000;
	this->my_weight[1][2] = 20000;
	this->my_weight[1][3] = 30000;
	this->my_weight[1][4] = 30000; // second turn's 4 is not important (making new move 4 is more valuable than making move 4 to move 5)
	this->my_weight[1][5] = 27000000; //fix
	this->my_weight[1][6] = 27000000;

	this->op_weight[0] = 100;
	this->op_weight[1] = 10000;
	this->op_weight[2] = 20000;
	this->op_weight[3] = 30000;
	this->op_weight[4] = 900000; // fix
	this->op_weight[5] = 900000; // fix
	this->op_weight[6] = 900000;

	this->cand_size_1 = cand_size_1;
	this->cand_size_2 = cand_size_2;
	this->discount_factor = discount_factor;
}

void Basic_BK_AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void Basic_BK_AI_player::init_live_line_count(){
	int tmp;
	for(int x = 0; x<BOARD_SIZE; x++){
		for(int y = 0; y<BOARD_SIZE; y++){
			this->my_live_line_count[x][y][0] = 0;
			//hor
			if((6<=x)&&(x<=12)) this->my_live_line_count[x][y][0] += 6;
			else if(x < 6) this->my_live_line_count[x][y][0] += (x+1);
			else this->my_live_line_count[x][y][0] += (BOARD_SIZE-x);
			//ver
			if((6<=y)&&(y<=12)) this->my_live_line_count[x][y][0] += 6;
			else if(y < 6) this->my_live_line_count[x][y][0] += (y+1);
			else this->my_live_line_count[x][y][0] += (BOARD_SIZE-y);
			//top left down right diag ---------------------------------------------------> this part should be improved
			tmp = 0;
			for(int z = 0; z < 6; z++){
				if( (x-z >= 0 && y+z < BOARD_SIZE) && (x-z+5 < BOARD_SIZE && y+z-5 >=0) ){
					tmp++;
				}
			}
			this->my_live_line_count[x][y][0] += tmp;
			//top right down left diag ---------------------------------------------------> this part should be improved
			tmp = 0;
			for(int z = 0; z < 6; z++){
				if( (x-z >= 0 && y-z >= 0) && (x-z+5 < BOARD_SIZE && y-z+5 < BOARD_SIZE) ){
					tmp++;
				}
			}
			this->my_live_line_count[x][y][0] += tmp;


			this->op_live_line_count[x][y][0] = this->my_live_line_count[x][y][0];
			for(int z = 1; z<7; z++){
				this->my_live_line_count[x][y][z] = this->op_live_line_count[x][y][z] = 0;
			}
		}
	}

	return;
}

void Basic_BK_AI_player::init_player(int my_color, int op_color){
	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_live_line_count();
	init_board(); // should be executed after init_live_line_count()

	return;
}

void Basic_BK_AI_player::set_forced_move(int color, int x, int y){
	pair< pair<int,int>, pair<int,int> > result;

	set_move_update_live_line(this->board, this->my_live_line_count, this->op_live_line_count, color, x, y);
	
	if(this->my_color == color){
		result.first.first = result.second.first = x;
		result.first.second = result.second.second = y;
		this->last_my_move = result;
	}

	return;
}

pair< pair<int, int>, pair<int, int> > Basic_BK_AI_player::return_fixed_start_move(){
	pair< pair<int, int>, pair<int, int> > result;
	result.first.first = BOARD_SIZE/2 - 2;
	result.first.second = BOARD_SIZE/2;
	result.second.first = BOARD_SIZE/2 - 1;
	result.second.second = BOARD_SIZE/2 + 1;

	return result;
}

int Basic_BK_AI_player::future_reward(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7]){
	int x,y,z,score,max,max_x,max_y,tmp1,tmp2;
	int scored_board[BOARD_SIZE][BOARD_SIZE];

	// best first move for op
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(board[x][y] == EMPTY){
				score = 0;
				for(z=0;z<7;z++){
					tmp1 = op_live_line_count[x][y][z]*this->my_weight[0][z];
					tmp2 = my_live_line_count[x][y][z]*this->op_weight[z];

					score += tmp1 + tmp2;

				}

				scored_board[x][y] = score;
			}
		}
	}

	max = -1000;
	max_x = -1;
	max_y = -1;
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if((board[x][y] == EMPTY) && (max < scored_board[x][y])){
				max = scored_board[x][y];
				max_x = x;
				max_y = y;
			}
		}
	}

	if(max == -1000) return 0;

	set_move_update_live_line(board, my_live_line_count, op_live_line_count, this->op_color, max_x, max_y);

	// best second move for op
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(board[x][y] == EMPTY){
				score = 0;
				for(z=0;z<7;z++){
					tmp1 = op_live_line_count[x][y][z]*this->my_weight[0][z];
					tmp2 = my_live_line_count[x][y][z]*this->op_weight[z];

					score += tmp1 + tmp2;

				}

				scored_board[x][y] = score;
			}
		}
	}

	max = -1000;
	max_x = -1;
	max_y = -1;
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if((board[x][y] == EMPTY) && (max < scored_board[x][y])){
				max = scored_board[x][y];
				max_x = x;
				max_y = y;
			}
		}
	}

	set_move_update_live_line(board, my_live_line_count, op_live_line_count, this->op_color, max_x, max_y);

	// best move for my
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(board[x][y] == EMPTY){
				score = 0;
				for(z=0;z<7;z++){
					tmp1 = op_live_line_count[x][y][z]*this->my_weight[0][z];
					tmp2 = my_live_line_count[x][y][z]*this->op_weight[z];

					score += tmp1 + tmp2;

				}

				scored_board[x][y] = score;
			}
		}
	}

	max = -1000;
	max_x = -1;
	max_y = -1;
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if((board[x][y] == EMPTY) && (max < scored_board[x][y])){
				max = scored_board[x][y];
				max_x = x;
				max_y = y;
			}
		}
	}

	////////////////////////////////////////// can customize
	if(max == -1000) return 0;
	else return max;

}

tuple<int,int,int> Basic_BK_AI_player::best_cand_for_move(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7]){
	int i,j,k,x,y,z,f,tmp1,tmp2,score;
	int tmp_board[BOARD_SIZE][BOARD_SIZE];
	int tmp_my_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
	int tmp_op_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
	int my_scored_board[BOARD_SIZE][BOARD_SIZE];
	vector< tuple<int,int,int,int,int> > top_k_pos;
	tuple<int,int,int,int,int> top_pos;

	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(board[x][y] == EMPTY){
				score = 0;
				for(z=0;z<7;z++){
					tmp1 = my_live_line_count[x][y][z]*this->my_weight[1][z];
					tmp2 = op_live_line_count[x][y][z]*this->op_weight[z];

					score += tmp1 + tmp2;

				}

				my_scored_board[x][y] = score;
			}
		}
	}	

	top_k_pos.push_back( make_tuple(-1000,-1,-1,-1,-1) );
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if((board[x][y] == EMPTY) && (get<0>(top_k_pos.back()) < my_scored_board[x][y]) ){
				if( top_k_pos.size() < this->cand_size_2 ) top_k_pos.push_back( make_tuple(my_scored_board[x][y], x, y, -1, -1) ); // use cand_size_2
				else{
					top_k_pos.pop_back();
					top_k_pos.push_back( make_tuple(my_scored_board[x][y], x, y, -1, -1) );
				}
				sort(top_k_pos.begin(), top_k_pos.end(), _compare);
			}
		}
	}
	
	while(get<0>(top_k_pos.back()) < 0) top_k_pos.pop_back();
	if(get<0>(top_k_pos.front()) >= this->op_weight[4]){
		while( get<0>(top_k_pos.back()) < this->op_weight[4] ) top_k_pos.pop_back();
	}

	for(int i=0;i<top_k_pos.size();i++){
		x = get<1>(top_k_pos[i]);
		y = get<2>(top_k_pos[i]);

		memcpy(tmp_board, board, sizeof(int)*BOARD_SIZE*BOARD_SIZE);
		memcpy(tmp_my_live_line_count, my_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);
		memcpy(tmp_op_live_line_count, op_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);

		set_move_update_live_line(tmp_board, tmp_my_live_line_count, tmp_op_live_line_count, this->my_color, x, y);

		f = future_reward(tmp_board, tmp_my_live_line_count, tmp_op_live_line_count);
		top_k_pos[i] = make_tuple( get<0>(top_k_pos[i]) + (int)(this->discount_factor * (float)(f)), x, y, -1, -1);
	}
	sort(top_k_pos.begin(), top_k_pos.end(), _compare);

	top_pos = top_k_pos.front();

	return make_tuple( get<0>(top_pos), get<1>(top_pos), get<2>(top_pos));

}

pair< pair<int, int>, pair<int, int> > Basic_BK_AI_player::return_whole_move(pair< pair<int, int>, pair<int, int> > last_op_move){
	bool flag;
	int x, y, z, max, max_x, max_y, tmp1, tmp2, score, top_score;

	int tmp_board[BOARD_SIZE][BOARD_SIZE];
	int tmp_my_live_line_count[BOARD_SIZE][BOARD_SIZE][7];
	int tmp_op_live_line_count[BOARD_SIZE][BOARD_SIZE][7];

	int my_scored_board[BOARD_SIZE][BOARD_SIZE];
	tuple<int,int,int> best_cand;
	pair< pair<int, int>, pair<int, int> > result;
	vector< tuple<int,int,int,int,int> > top_k_pos; // tuple : score, x, y & desc order

	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(this->board[x][y] == EMPTY){
				score = 0;
				for(z=0;z<7;z++){
					tmp1 = this->my_live_line_count[x][y][z]*this->my_weight[0][z];
					tmp2 = this->op_live_line_count[x][y][z]*this->op_weight[z];

					score += tmp1 + tmp2;

				}

				my_scored_board[x][y] = score;
			}
		}
	}

	//get top-k element
	top_k_pos.push_back( make_tuple(-1000,-1,-1,-1,-1) );
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if( (this->board[x][y] == EMPTY) && (get<0>(top_k_pos.back()) < my_scored_board[x][y]) ){
				if( top_k_pos.size() < this->cand_size_1 ) top_k_pos.push_back( make_tuple(my_scored_board[x][y], x, y, -1, -1) );
				else{
					top_k_pos.pop_back();
					top_k_pos.push_back( make_tuple(my_scored_board[x][y], x, y, -1, -1) );
				}
				sort(top_k_pos.begin(), top_k_pos.end(), _compare);
			}
		}
	}

	while(get<0>(top_k_pos.back()) < 0) top_k_pos.pop_back();
	if(get<0>(top_k_pos.front()) >= this->op_weight[4]){
		while( get<0>(top_k_pos.back()) < this->op_weight[4] ) top_k_pos.pop_back();
	}

//=====================SIMULATION======================
/*
	// make two pairs from top_k_pos
	int len = top_k_pos.size();
	for( int i = 0; i < len; i++ ) {
		for ( int j = 0; j < len; j++ ) {
			if ( i == j ) continue;
			int x1 = get<1>( top_k_pos[i] );
			int y1 = get<2>( top_k_pos[i] );
			int x2 = get<1>( top_k_pos[j] );
			int y2 = get<2>( top_k_pos[j] );

			// copy current state
			memcpy(tmp_board, this->board, sizeof(int)*BOARD_SIZE*BOARD_SIZE);
			memcpy(tmp_my_live_line_count, this->my_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);
			memcpy(tmp_op_live_line_count, this->op_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);

			set_move_update_live_line( tmp_board, tmp_my_live_line_count, tmp_op_live_line_count, this->my_color, x1, y1 );
			set_move_update_live_line( tmp_board, tmp_my_live_line_count, tmp_op_live_line_count, this->my_color, x2, y2 );

			


		}
	}*/
			

	//start simul for each move
	for(int i=0;i<top_k_pos.size();i++){
		// candidate
		x = get<1>(top_k_pos[i]);
		y = get<2>(top_k_pos[i]);

		// copy current state
		memcpy(tmp_board, this->board, sizeof(int)*BOARD_SIZE*BOARD_SIZE);
		memcpy(tmp_my_live_line_count, this->my_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);
		memcpy(tmp_op_live_line_count, this->op_live_line_count, sizeof(int)*BOARD_SIZE*BOARD_SIZE*7);

		// update threat
		set_move_update_live_line(tmp_board, tmp_my_live_line_count, tmp_op_live_line_count, this->my_color, x, y);

		// 
		best_cand = best_cand_for_move(tmp_board, tmp_my_live_line_count, tmp_op_live_line_count);
		top_k_pos[i] = make_tuple( get<0>(top_k_pos[i]) + (int)(this->discount_factor * (float)(get<0>(best_cand))), x, y, get<1>(best_cand), get<2>(best_cand));
	}
	sort(top_k_pos.begin(), top_k_pos.end(), _compare);


	result.first.first = get<1>(top_k_pos.front());
	result.first.second = get<2>(top_k_pos.front());
	result.second.first = get<3>(top_k_pos.front());
	result.second.second = get<4>(top_k_pos.front());

	return result;
}

pair< pair<int, int>, pair<int, int> > Basic_BK_AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){

	pair< pair<int, int>, pair<int, int> > result;

	// save opponent's move
	if(!(this->my_color == WHITE && this->turn == 0)){
		set_move_update_live_line( this->board, this->my_live_line_count, this->op_live_line_count, this->op_color , last_op_move.first.first, last_op_move.first.second);
		set_move_update_live_line( this->board, this->my_live_line_count, this->op_live_line_count, this->op_color , last_op_move.second.first, last_op_move.second.second);
	}

	if(this->my_color == WHITE && this->turn == 0) result = return_fixed_start_move();
	else result = return_whole_move(last_op_move); 

	set_move_update_live_line( this->board, this->my_live_line_count, this->op_live_line_count, this->my_color , result.first.first, result.first.second);
	set_move_update_live_line( this->board, this->my_live_line_count, this->op_live_line_count, this->my_color , result.second.first, result.second.second);

	// save player's move
	this->turn++;
	this->last_my_move = result;
	return result;
}

void Basic_BK_AI_player::set_move_update_live_line(int board[][BOARD_SIZE], int my_live_line_count[][BOARD_SIZE][7], int op_live_line_count[][BOARD_SIZE][7], int color, int x, int y){ // should add checking over 6
	int current_my_count, current_op_count, current_my_color, current_op_color, tmp, b;
	int (*current_my_live_line_count)[BOARD_SIZE][7];
	int (*current_op_live_line_count)[BOARD_SIZE][7];

	if(color == this->my_color){
		current_my_live_line_count = my_live_line_count;
		current_op_live_line_count = op_live_line_count;
		current_my_color = this->my_color;
		current_op_color = this->op_color;
	}else{
		current_my_live_line_count = op_live_line_count;
		current_op_live_line_count = my_live_line_count;
		current_my_color = this->op_color;
		current_op_color = this->my_color;
	}

	// set move
	if(board[x][y] != EMPTY){
		cout << "error:basic_bk_ai_player received overlapped move" << endl;
		return;
	}

	//============================================= check horizontal live4,5 =============================================
	for( int i = ((x-5 > 0) ? x-5 : 0); i<=x; i++){
		if( i+5 >= BOARD_SIZE) continue;

		// check my live line first
		current_my_count = 0;
		if( ( i-1 < 0 || board[i-1][y]!=current_my_color ) && ( i+6 >= BOARD_SIZE || board[i+6][y]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(board[i+b][y] == current_my_color) current_my_count++;
				else if(board[i+b][y] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(board[i+b][y] == EMPTY){
						current_my_live_line_count[i+b][y][current_my_count] -= 1;
						current_my_live_line_count[i+b][y][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( i-1 < 0 || board[i-1][y]!=current_op_color ) && ( i+6 >= BOARD_SIZE || board[i+6][y]!=current_op_color) ){  //----------------------check later
			for(b=0; b<6 ;b++){
				if(board[i+b][y] == current_op_color) current_op_count++;
				else if(board[i+b][y] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(board[i+b][y] == EMPTY){
						current_op_live_line_count[i+b][y][current_op_count] -= 1;
					}
				}
			}
		} 
	}

	//============================================= check vertical live4,5 =============================================
	for( int j = ((y-5 > 0) ? y-5 : 0); j<=y; j++){
		if( j+5 >= BOARD_SIZE) continue;

		// check my live line first
		current_my_count = 0;
		if( ( j-1 < 0 || board[x][j-1]!=current_my_color ) && ( j+6 >= BOARD_SIZE || board[x][j+6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(board[x][j+b] == current_my_color) current_my_count++;
				else if(board[x][j+b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x][j+b] == EMPTY){
						current_my_live_line_count[x][j+b][current_my_count] -= 1;
						current_my_live_line_count[x][j+b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( j-1 < 0 || board[x][j-1]!=current_op_color ) && ( j+6 >= BOARD_SIZE || board[x][j+6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(board[x][j+b] == current_op_color) current_op_count++;
				else if(board[x][j+b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x][j+b] == EMPTY){
						current_op_live_line_count[x][j+b][current_op_count] -= 1;
					}
				}
			}
		} 
	}

	//============================================= check top left down right diag live4,5 =============================================
	tmp = ( BOARD_SIZE - y - 1 < x ) ? BOARD_SIZE - y - 1 : x;
	if(tmp > 5) tmp = 5;
	for( int i = 0; i <= tmp; i++){
		if( (x-i+5 >= BOARD_SIZE) || (y+i-5 < 0)) continue;

		// check my live line first
		current_my_count = 0;
		if( ( x-i-1 < 0 || y+i+1 >= BOARD_SIZE || board[x-i-1][y+i+1]!=current_my_color ) && ( x-i+6 >= BOARD_SIZE || y+i-6 < 0 || board[x-i+6][y+i-6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(board[x-i+b][y+i-b] == current_my_color) current_my_count++;
				else if(board[x-i+b][y+i-b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x-i+b][y+i-b] == EMPTY){
						current_my_live_line_count[x-i+b][y+i-b][current_my_count] -= 1;
						current_my_live_line_count[x-i+b][y+i-b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( x-i-1 < 0 || y+i+1 >= BOARD_SIZE || board[x-i-1][y+i+1]!=current_op_color ) && ( x-i+6 >= BOARD_SIZE || y+i-6 < 0 || board[x-i+6][y+i-6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(board[x-i+b][y+i-b] == current_op_color) current_op_count++;
				else if(board[x-i+b][y+i-b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x-i+b][y+i-b] == EMPTY){
						current_op_live_line_count[x-i+b][y+i-b][current_op_count] -= 1;
					}
				}
			}
		} 
	}

	//============================================= check top right down left diag live4,5 =============================================
	tmp = ( x < y ) ? x : y;
	if(tmp > 5) tmp = 5;
	for( int i = 0; i <= tmp; i++){
		if( (x-i+5 >= BOARD_SIZE) || (y-i+5 >= BOARD_SIZE)) continue;

		// check my live line first
		current_my_count = 0;
		if( ( x-i-1 < 0 || y-i-1 < 0 || board[x-i-1][y-i-1]!=current_my_color ) && ( x-i+6 >= BOARD_SIZE || y-i+6 >= BOARD_SIZE || board[x-i+6][y-i+6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(board[x-i+b][y-i+b] == current_my_color) current_my_count++;
				else if(board[x-i+b][y-i+b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x-i+b][y-i+b] == EMPTY){
						current_my_live_line_count[x-i+b][y-i+b][current_my_count] -= 1;
						current_my_live_line_count[x-i+b][y-i+b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( x-i-1 < 0 || y-i-1 < 0 || board[x-i-1][y-i-1]!=current_op_color ) && ( x-i+6 >= BOARD_SIZE || y-i+6 >= BOARD_SIZE || board[x-i+6][y-i+6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(board[x-i+b][y-i+b] == current_op_color) current_op_count++;
				else if(board[x-i+b][y-i+b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(board[x-i+b][y-i+b] == EMPTY){
						current_op_live_line_count[x-i+b][y-i+b][current_op_count] -= 1;
					}
				}
			}
		} 
	}


	//==================================================== if move make exist live line to over 6 -> remove exist live line ====================================================
	//left hor x-1,y ~ x-6
	current_my_count = 0;
	if( (x-6 >= 0) && (x-7 < 0 || board[x-7][y]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x-b][y] == current_my_color) current_my_count++;
			else if(board[x-b][y] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x-b][y] == EMPTY){
					current_my_live_line_count[x-b][y][current_my_count] -= 1;
				}
			}
		}
	}

	//right hor x+1,y ~ x+6,y
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE) && (x+7 >= BOARD_SIZE || board[x+7][y]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x+b][y] == current_my_color) current_my_count++;
			else if(board[x+b][y] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x+b][y] == EMPTY){
					current_my_live_line_count[x+b][y][current_my_count] -= 1;
				}
			}
		}
	}

	//upside ver x,y+1 ~ x, y+6
	current_my_count = 0;
	if( (y+6 < BOARD_SIZE) && (y+7 >= BOARD_SIZE || board[x][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x][y+b] == current_my_color) current_my_count++;
			else if(board[x][y+b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x][y+b] == EMPTY){
					current_my_live_line_count[x][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside ver x,y-1 ~ x, y-6
	current_my_count = 0;
	if( (y-6 >= 0) && (y-7 < 0 || board[x][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x][y-b] == current_my_color) current_my_count++;
			else if(board[x][y-b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x][y-b] == EMPTY){
					current_my_live_line_count[x][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	//upside top left down right x-1,y+1 ~ x-6,y+6
	current_my_count = 0;
	if( (x-6 >= 0 && y+6 < BOARD_SIZE) && (x-7 < 0 || y+7 >= BOARD_SIZE || board[x-7][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x-b][y+b] == current_my_color) current_my_count++;
			else if(board[x-b][y+b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x-b][y+b] == EMPTY){
					current_my_live_line_count[x-b][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside top left down right x+1,y-1 ~ x+6,y-6
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE && y-6 >= 0) && (x+7 >= BOARD_SIZE || y-7 < 0 || board[x+7][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x+b][y-b] == current_my_color) current_my_count++;
			else if(board[x+b][y-b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x+b][y-b] == EMPTY){
					current_my_live_line_count[x+b][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	//upside top right down left x+1,y+1 ~ x+6,y+6
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE && y+6 < BOARD_SIZE) && (x+7 >= BOARD_SIZE || y+7 >= BOARD_SIZE || board[x+7][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x+b][y+b] == current_my_color) current_my_count++;
			else if(board[x+b][y+b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x+b][y+b] == EMPTY){
					current_my_live_line_count[x+b][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside top right down left x-1,y-1 ~ x-6,y-6
	current_my_count = 0;
	if( (x-6 >= 0 && y-6 >= 0) && (x-7 < 0 || y-7 < 0 || board[x-7][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(board[x-b][y-b] == current_my_color) current_my_count++;
			else if(board[x-b][y-b] == current_op_color){
				current_my_count = -1;
				break;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(board[x-b][y-b] == EMPTY){
					current_my_live_line_count[x-b][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	// set move
	board[x][y] = current_my_color;

	return;
}