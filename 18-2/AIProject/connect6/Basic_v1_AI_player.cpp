#include <utility>
#include <cstdlib>
#include <iostream>
#include "AI_player.h"
#include "Basic_v1_AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

void Basic_v1_AI_player::debug_print_my_live_line_count(int ll){
	cout << "================================================ my " << ll << " ================================================" << endl;
	for(int y=BOARD_SIZE - 1; y>=0;y--){
		for(int x=0; x < BOARD_SIZE; x++){
			cout << my_live_line_count[x][y][ll] << "	";
		}
		cout << endl;
	}
}

void Basic_v1_AI_player::debug_print_op_live_line_count(int ll){
	cout << "================================================ op " << ll << " ================================================" << endl;
	for(int y=BOARD_SIZE - 1; y>=0;y--){
		for(int x=0; x < BOARD_SIZE; x++){
			cout << op_live_line_count[x][y][ll] << "	";
		}
		cout << endl;
	}
}


Basic_v1_AI_player::Basic_v1_AI_player(){

	// weight for first half move
	this->my_weight[0][0] = 500;
	this->my_weight[0][1] = 10000;
	this->my_weight[0][2] = 20000;
	this->my_weight[0][3] = 30000; //fix
	this->my_weight[0][4] = 27000000; //fix
	this->my_weight[0][5] = 27000000; //fix
	this->my_weight[0][6] = 0;

	// weight for second half move
	this->my_weight[1][0] = 40;
	this->my_weight[1][1] = 5000;
	this->my_weight[1][2] = 10000;
	this->my_weight[1][3] = 30000;
	this->my_weight[1][4] = 10000; // second turn's 4 is not important (making new move 4 is more valuable than making move 4 to move 5)
	this->my_weight[1][5] = 27000000; //fix
	this->my_weight[1][6] = 0;

	this->op_weight[0] = 350;
	this->op_weight[1] = 7000;
	this->op_weight[2] = 150000;
	this->op_weight[3] = 20000;
	this->op_weight[4] = 900000; // fix
	this->op_weight[5] = 900000; // fix
	this->op_weight[6] = 0;

}

Basic_v1_AI_player::Basic_v1_AI_player(float my_0_0, float my_0_1, float my_0_2, float my_0_3, float my_0_4, float my_0_5, float my_0_6,
									float my_1_0, float my_1_1, float my_1_2, float my_1_3, float my_1_4, float my_1_5, float my_1_6,
									float op_0, float op_1, float op_2, float op_3, float op_4, float op_5, float op_6){
	// weight for first half move
	this->my_weight[0][0] = my_0_0;
	this->my_weight[0][1] = my_0_1;
	this->my_weight[0][2] = my_0_2;
	this->my_weight[0][3] = my_0_3;
	this->my_weight[0][4] = my_0_4;
	this->my_weight[0][5] = my_0_5;
	this->my_weight[0][6] = my_0_6;

	// weight for second half move
	this->my_weight[1][0] = my_1_0;
	this->my_weight[1][1] = my_1_1;
	this->my_weight[1][2] = my_1_2;
	this->my_weight[1][3] = my_1_3;
	this->my_weight[1][4] = my_1_4;
	this->my_weight[1][5] = my_1_5;
	this->my_weight[1][6] = my_1_6;

	this->op_weight[0] = op_0;
	this->op_weight[1] = op_1;
	this->op_weight[2] = op_2;
	this->op_weight[3] = op_3;
	this->op_weight[4] = op_4;
	this->op_weight[5] = op_5;
	this->op_weight[6] = op_6;

}

void Basic_v1_AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void Basic_v1_AI_player::init_live_line_count(){
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

void Basic_v1_AI_player::init_player(int my_color, int op_color){
	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_live_line_count();
	init_board(); // should be executed after init_live_line_count()

	return;
}

void Basic_v1_AI_player::set_forced_move(int color, int x, int y){
	pair< pair<int,int>, pair<int,int> > result;

	set_move_update_live_line(color, x, y);
	
	if(this->my_color == color){
		result.first.first = result.second.first = x;
		result.first.second = result.second.second = y;
		this->last_my_move = result;
	}

	return;
}

pair< pair<int, int>, pair<int, int> > Basic_v1_AI_player::return_fixed_start_move(){
	pair< pair<int, int>, pair<int, int> > result;
	result.first.first = BOARD_SIZE/2 - 2;
	result.first.second = BOARD_SIZE/2;
	result.second.first = BOARD_SIZE/2 - 1;
	result.second.second = BOARD_SIZE/2 + 1;

	return result;
}

pair<int, int> Basic_v1_AI_player::return_half_move(pair< pair<int, int>, pair<int, int> > last_op_move, int half_turn){
	int x, y, z, max, max_x, max_y, tmp;
	pair<int, int> result;

	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(this->board[x][y] == EMPTY){
				tmp = 0;
				for(z=0;z<7;z++){
					tmp += this->my_live_line_count[x][y][z]*this->my_weight[half_turn][z] + this->op_live_line_count[x][y][z]*this->op_weight[z];
				}

				this->scored_board[x][y] = tmp;
			}
		}
	}

	max = -1;
	for(x=0;x<BOARD_SIZE;x++){
		for(y=0;y<BOARD_SIZE;y++){
			if(this->board[x][y] == EMPTY){
				if(this->scored_board[x][y] > max){
					max = this->scored_board[x][y];
					max_x = x;
					max_y = y;
				}
			}
		}
	}

	result.first = max_x;
	result.second = max_y;

	return result;
}

pair< pair<int, int>, pair<int, int> > Basic_v1_AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){

	pair< pair<int, int>, pair<int, int> > result;

	// save opponent's move
	if(!(this->my_color == WHITE && this->turn == 0)){
		set_move_update_live_line( this->op_color , last_op_move.first.first, last_op_move.first.second);
		set_move_update_live_line( this->op_color , last_op_move.second.first, last_op_move.second.second);
	}

	if(this->my_color == WHITE && this->turn == 0){
		result = return_fixed_start_move();
		set_move_update_live_line( this->my_color , result.first.first, result.first.second);
		set_move_update_live_line( this->my_color , result.second.first, result.second.second);
	}else{
		result.first = return_half_move(last_op_move,0);
		set_move_update_live_line( this->my_color , result.first.first, result.first.second);

		result.second = return_half_move(last_op_move,1);
		set_move_update_live_line( this->my_color , result.second.first, result.second.second);
	}

	// save player's move
	this->turn++;
	this->last_my_move = result;
	return result;
}

void Basic_v1_AI_player::set_move_update_live_line(int color, int x, int y){ // should add checking over 6
	int current_my_count, current_op_count, current_my_color, current_op_color, tmp, b;
	int (*current_my_live_line_count)[BOARD_SIZE][7];
	int (*current_op_live_line_count)[BOARD_SIZE][7];

	if(color == this->my_color){
		current_my_live_line_count = this->my_live_line_count;
		current_op_live_line_count = this->op_live_line_count;
		current_my_color = this->my_color;
		current_op_color = this->op_color;
	}else{
		current_my_live_line_count = this->op_live_line_count;
		current_op_live_line_count = this->my_live_line_count;
		current_my_color = this->op_color;
		current_op_color = this->my_color;
	}

	// set move
	if(this->board[x][y] != EMPTY){
		cout << "error:basic_ai_player received overlapped move from simulator" << endl;
		return;
	}

	//============================================= check horizontal live4,5 =============================================
	for( int i = ((x-5 > 0) ? x-5 : 0); i<=x; i++){
		if( i+5 >= BOARD_SIZE) continue;

		// check my live line first
		current_my_count = 0;
		if( ( i-1 < 0 || this->board[i-1][y]!=current_my_color ) && ( i+6 >= BOARD_SIZE || this->board[i+6][y]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(this->board[i+b][y] == current_my_color) current_my_count++;
				else if(this->board[i+b][y] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[i+b][y] == EMPTY){
						current_my_live_line_count[i+b][y][current_my_count] -= 1;
						current_my_live_line_count[i+b][y][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( i-1 < 0 || this->board[i-1][y]!=current_op_color ) && ( i+6 >= BOARD_SIZE || this->board[i+6][y]!=current_op_color) ){  //----------------------check later
			for(b=0; b<6 ;b++){
				if(this->board[i+b][y] == current_op_color) current_op_count++;
				else if(this->board[i+b][y] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[i+b][y] == EMPTY){
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
		if( ( j-1 < 0 || this->board[x][j-1]!=current_my_color ) && ( j+6 >= BOARD_SIZE || this->board[x][j+6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(this->board[x][j+b] == current_my_color) current_my_count++;
				else if(this->board[x][j+b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x][j+b] == EMPTY){
						current_my_live_line_count[x][j+b][current_my_count] -= 1;
						current_my_live_line_count[x][j+b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( j-1 < 0 || this->board[x][j-1]!=current_op_color ) && ( j+6 >= BOARD_SIZE || this->board[x][j+6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(this->board[x][j+b] == current_op_color) current_op_count++;
				else if(this->board[x][j+b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x][j+b] == EMPTY){
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
		if( ( x-i-1 < 0 || y+i+1 >= BOARD_SIZE || this->board[x-i-1][y+i+1]!=current_my_color ) && ( x-i+6 >= BOARD_SIZE || y+i-6 < 0 || this->board[x-i+6][y+i-6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(this->board[x-i+b][y+i-b] == current_my_color) current_my_count++;
				else if(this->board[x-i+b][y+i-b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x-i+b][y+i-b] == EMPTY){
						current_my_live_line_count[x-i+b][y+i-b][current_my_count] -= 1;
						current_my_live_line_count[x-i+b][y+i-b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( x-i-1 < 0 || y+i+1 >= BOARD_SIZE || this->board[x-i-1][y+i+1]!=current_op_color ) && ( x-i+6 >= BOARD_SIZE || y+i-6 < 0 || this->board[x-i+6][y+i-6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(this->board[x-i+b][y+i-b] == current_op_color) current_op_count++;
				else if(this->board[x-i+b][y+i-b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x-i+b][y+i-b] == EMPTY){
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
		if( ( x-i-1 < 0 || y-i-1 < 0 || this->board[x-i-1][y-i-1]!=current_my_color ) && ( x-i+6 >= BOARD_SIZE || y-i+6 >= BOARD_SIZE || this->board[x-i+6][y-i+6]!=current_my_color) ){  // if not, it means over 6
			for(b=0; b<6 ;b++){
				if(this->board[x-i+b][y-i+b] == current_my_color) current_my_count++;
				else if(this->board[x-i+b][y-i+b] == current_op_color){
					current_my_count = -1;
					break;
				}
			}

			if(current_my_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x-i+b][y-i+b] == EMPTY){
						current_my_live_line_count[x-i+b][y-i+b][current_my_count] -= 1;
						current_my_live_line_count[x-i+b][y-i+b][current_my_count+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		current_op_count = 0;
		if( ( x-i-1 < 0 || y-i-1 < 0 || this->board[x-i-1][y-i-1]!=current_op_color ) && ( x-i+6 >= BOARD_SIZE || y-i+6 >= BOARD_SIZE || this->board[x-i+6][y-i+6]!=current_op_color) ){
			for(b=0; b<6 ;b++){
				if(this->board[x-i+b][y-i+b] == current_op_color) current_op_count++;
				else if(this->board[x-i+b][y-i+b] == current_my_color){
					current_op_count = -1;
					break;
				}
			}

			if(current_op_count != -1){
				for(b=0; b<6 ;b++){
					if(this->board[x-i+b][y-i+b] == EMPTY){
						current_op_live_line_count[x-i+b][y-i+b][current_op_count] -= 1;
					}
				}
			}
		} 
	}


	//==================================================== if move make exist live line to over 6 -> remove exist live line ====================================================
	//left hor x-1,y ~ x-6
	current_my_count = 0;
	if( (x-6 >= 0) && (x-7 < 0 || this->board[x-7][y]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x-b][y] == current_my_color) current_my_count++;
			else if(this->board[x-b][y] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x-b][y] == EMPTY){
					current_my_live_line_count[x-b][y][current_my_count] -= 1;
				}
			}
		}
	}

	//right hor x+1,y ~ x+6,y
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE) && (x+7 >= BOARD_SIZE || this->board[x+7][y]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x+b][y] == current_my_color) current_my_count++;
			else if(this->board[x+b][y] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x+b][y] == EMPTY){
					current_my_live_line_count[x+b][y][current_my_count] -= 1;
				}
			}
		}
	}

	//upside ver x,y+1 ~ x, y+6
	current_my_count = 0;
	if( (y+6 < BOARD_SIZE) && (y+7 >= BOARD_SIZE || this->board[x][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x][y+b] == current_my_color) current_my_count++;
			else if(this->board[x][y+b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x][y+b] == EMPTY){
					current_my_live_line_count[x][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside ver x,y-1 ~ x, y-6
	current_my_count = 0;
	if( (y-6 >= 0) && (y-7 < 0 || this->board[x][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x][y-b] == current_my_color) current_my_count++;
			else if(this->board[x][y-b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x][y-b] == EMPTY){
					current_my_live_line_count[x][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	//upside top left down right x-1,y+1 ~ x-6,y+6
	current_my_count = 0;
	if( (x-6 >= 0 && y+6 < BOARD_SIZE) && (x-7 < 0 || y+7 >= BOARD_SIZE || this->board[x-7][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x-b][y+b] == current_my_color) current_my_count++;
			else if(this->board[x-b][y+b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x-b][y+b] == EMPTY){
					current_my_live_line_count[x-b][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside top left down right x+1,y-1 ~ x+6,y-6
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE && y-6 >= 0) && (x+7 >= BOARD_SIZE || y-7 < 0 || this->board[x+7][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x+b][y-b] == current_my_color) current_my_count++;
			else if(this->board[x+b][y-b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x+b][y-b] == EMPTY){
					current_my_live_line_count[x+b][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	//upside top right down left x+1,y+1 ~ x+6,y+6
	current_my_count = 0;
	if( (x+6 < BOARD_SIZE && y+6 < BOARD_SIZE) && (x+7 >= BOARD_SIZE || y+7 >= BOARD_SIZE || this->board[x+7][y+7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x+b][y+b] == current_my_color) current_my_count++;
			else if(this->board[x+b][y+b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x+b][y+b] == EMPTY){
					current_my_live_line_count[x+b][y+b][current_my_count] -= 1;
				}
			}
		}
	}

	//downside top right down left x-1,y-1 ~ x-6,y-6
	current_my_count = 0;
	if( (x-6 >= 0 && y-6 >= 0) && (x-7 < 0 || y-7 < 0 || this->board[x-7][y-7]!=current_my_color) ){
		for(b=1;b<=6;b++){
			if(this->board[x-b][y-b] == current_my_color) current_my_count++;
			else if(this->board[x-b][y-b] == current_op_color){
				current_my_count = -1;
			}
		}

		if(current_my_count != -1){
			for(b=1;b<=6;b++){
				if(this->board[x-b][y-b] == EMPTY){
					current_my_live_line_count[x-b][y-b][current_my_count] -= 1;
				}
			}
		}
	}

	// set move
	this->board[x][y] = current_my_color;

	return;
}