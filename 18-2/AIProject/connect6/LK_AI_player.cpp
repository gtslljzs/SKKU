#include <utility>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "AI_player.h"
#include "LK_AI_player.h"
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

LK_AI_player::LK_AI_player(){
	this->dx[0] = -1;
	this->dx[1] = -1;
	this->dx[2] = -1;
	this->dx[3] = 0;
	this->dx[4] = 1;
	this->dx[5] = 1;
	this->dx[6] = 1;
	this->dx[7] = 0;

	this->dy[0] = -1;
	this->dy[1] = 0;
	this->dy[2] = 1;
	this->dy[3] = 1;
	this->dy[4] = 1;
	this->dy[5] = 0;
	this->dy[6] = -1;
	this->dy[7] = -1;

	this->attack_penalty = 0.85;
	this->four_penalty = 3.5;
	this->future_penalty = 1.2;
	this->cand_size = 5;
}

LK_AI_player::LK_AI_player(double attack_penalty, double four_penalty, double future_penalty, int cand_size){
	this->dx[0] = -1;
	this->dx[1] = -1;
	this->dx[2] = -1;
	this->dx[3] = 0;
	this->dx[4] = 1;
	this->dx[5] = 1;
	this->dx[6] = 1;
	this->dx[7] = 0;

	this->dy[0] = -1;
	this->dy[1] = 0;
	this->dy[2] = 1;
	this->dy[3] = 1;
	this->dy[4] = 1;
	this->dy[5] = 0;
	this->dy[6] = -1;
	this->dy[7] = -1;

	this->attack_penalty = attack_penalty;
	this->four_penalty = four_penalty;
	this->future_penalty = future_penalty;
	this->cand_size = cand_size;	
}

void LK_AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void LK_AI_player::init_player(int my_color, int op_color){
	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_board();

	return;
}

void LK_AI_player::new_stone(int color, int x, int y){
	this->board[x][y] = color;
	if(color == this->my_color) my_stone.push_back(make_pair(x, y));
	else op_stone.push_back(make_pair(x,y));
	modify_weight();
}

void LK_AI_player::set_forced_move(int color, int x, int y){
	int x_reversed, y_reversed;
	// reversed_input
	x_reversed = x;
	y_reversed = BOARD_SIZE - y - 1;

	new_stone(color, x_reversed, y_reversed);
	return;
}

vector<pair<int,pair<int,int> > > LK_AI_player::max_weight(int cand_size){
	vector<pair<int,pair<int,int> > > ret_x_y;
	vector<pair<int,pair<int,int> > > temp_x_y;
	for(int i=0;i<BOARD_SIZE;i++)
		for(int j=0;j<BOARD_SIZE;j++)
			if(this->board_weight_defense[i][j] + this->board_weight_attack[i][j]){
				int z = this->board_weight_defense[i][j] + this->board_weight_attack[i][j];
				int x = i, y = j;
				temp_x_y.push_back(make_pair(z,make_pair(x,y)));
			}
	sort(temp_x_y.begin(),temp_x_y.end());
	int temp_size = temp_x_y.size();
	for(int i=temp_size-1;i>=max(0,temp_size-cand_size);i--){
		int z = temp_x_y[i].first;
		int x = temp_x_y[i].second.first;
		int y = temp_x_y[i].second.second;
		ret_x_y.push_back(make_pair(z,make_pair(x,y)));
	}
	return ret_x_y;	
}

pair<int,pair<int,int> > LK_AI_player::best_move(int num_move){
	pair<int,pair<int,int> > ret_x_y;
	ret_x_y.second.first = -1;
	ret_x_y.second.second = -1;
	int max_score = -1000;
	if(num_move == 2){
		vector<pair<int,pair<int,int> > > my_cand_first;
		my_cand_first = max_weight(cand_size);
		int my_cand_first_size = my_cand_first.size();
		for(int i=0;i<my_cand_first_size;i++){
			int x_1 = my_cand_first[i].second.first;
			int y_1 = my_cand_first[i].second.second;
			int score = my_cand_first[i].first;
			new_stone(this->my_color,x_1,y_1);
			score *= max(1,(int)(this->attack_penalty*check_four(this->my_color)));
			score += best_move(num_move-1).first;
			if(score > max_score){
				max_score = score;
				ret_x_y.second.first = x_1;
				ret_x_y.second.second = y_1;
			}
			this->my_stone.pop_back();
			this->board[x_1][y_1] = 0;
		}
	}else if(num_move == 1){
		vector<pair<int,pair<int,int> > > my_cand_second;
		my_cand_second = max_weight(cand_size);
		int my_cand_second_size = my_cand_second.size();
		for(int i=0;i<my_cand_second_size;i++){
			int x_2 = my_cand_second[i].second.first;
			int y_2 = my_cand_second[i].second.second;
			int score = my_cand_second[i].first;
			new_stone(this->my_color,x_2,y_2);
			score *= max(1,(int)(this->attack_penalty*check_four(this->my_color)));
			score -= (int)(this->future_penalty * best_move(num_move-1).first);
			if(score > max_score){
				//printf("%d %d -> %d\n",x_2,y_2,score);
				max_score = score;
				ret_x_y.second.first = x_2;
				ret_x_y.second.second = y_2;
				ret_x_y.first = score;
			}
			this->my_stone.pop_back();
			this->board[x_2][y_2] = 0;
		}
	}else{
		vector<pair<int,pair<int,int> > > op_cand_first;
		op_cand_first = max_weight(cand_size);
		int op_cand_first_size = op_cand_first.size();
		for(int i=0;i<op_cand_first_size;i++){
			int x_1 = op_cand_first[i].second.first;
			int y_1 = op_cand_first[i].second.second;
			int score_1 = op_cand_first[i].first;
			new_stone(this->op_color,x_1,y_1);
			score_1 *= max(1,check_four(this->op_color));
			vector<pair<int,pair<int,int> > > op_cand_second;
			op_cand_second = max_weight(cand_size);
			int op_cand_second_size = op_cand_second.size();
			for(int j=0;j<op_cand_second_size;j++){
				int x_2 = op_cand_second[j].second.first;
				int y_2 = op_cand_second[j].second.second;
				int score_2 = op_cand_second[j].first;
				new_stone(this->op_color,x_2,y_2);
				score_2 *= max(1,check_four(this->op_color));
				int score = score_1 + score_2;
				if(score > max_score){
					max_score = score;
					ret_x_y.first = score;
				}
				this->op_stone.pop_back();
				this->board[x_2][y_2] = 0;
			}
			this->op_stone.pop_back();
			this->board[x_1][y_1] = 0;
		}
	}
	return ret_x_y;
}

void LK_AI_player::modify_weight(){
	memset(this->board_weight_defense,0,sizeof(this->board_weight_defense));
	memset(this->board_weight_attack,0,sizeof(this->board_weight_attack));
	for(int x=0;x<BOARD_SIZE;x++)
		for(int y=0;y<BOARD_SIZE;y++)
			if(this->board[x][y]){
				for(int i=0; i < 4 ;i++){
					for(int j=0; j < WINDOW ;j++){
						vector<pair<int,int> > temp;
						int left = j, right = WINDOW-1-j;
						int cx = x, cy = y;
						while(left --){
							cx += this->dx[i];
							cy += this->dy[i];
							if(cx < 0 || cx >= BOARD_SIZE || cy < 0 || cy >= BOARD_SIZE)
								break;
							temp.push_back(make_pair(cx,cy));
						}
						cx = x; cy = y;
						while(right --){
							cx += dx[i+4];
							cy += dy[i+4];
							if(cx < 0 || cx >= BOARD_SIZE || cy < 0 || cy >= BOARD_SIZE)
								break;
							temp.push_back(make_pair(cx,cy));
						}
					
						if(temp.size() < WINDOW - 1)continue;	
						int sx = x + (j+1) * this->dx[i];
						int sy = y + (j+1) * this->dy[i];
						if(!(sx < 0 || sx >= BOARD_SIZE || sy < 0 || sy >= BOARD_SIZE))
							if(this->board[sx][sy] == this->board[x][y])
								continue;
						sx = x + (WINDOW-j) * this->dx[i+4];
						sy = y + (WINDOW-j) * this->dy[i+4];
						if(!(sx < 0 || sx >= BOARD_SIZE || sy < 0 || sy >= BOARD_SIZE))
							if(this->board[sx][sy] == this->board[x][y])
								continue;
						bool active_threat = true;
						int connect_cnt = 1;
						for(int k=0; k < temp.size() ;k++){
							int tx = temp[k].first;
							int ty = temp[k].second;
							if(this->board[tx][ty] && this->board[tx][ty] != this->board[x][y])
								active_threat = false;
							if(this->board[tx][ty] == this->board[x][y])
								connect_cnt ++;
						}
						if(active_threat){
							for(int k=0; k < temp.size() ;k++){
								int tx = temp[k].first;
								int ty = temp[k].second;
								if(!this->board[tx][ty]){
									if(this->board[x][y] == this->my_color){
										if(connect_cnt >= 4)
											this->board_weight_attack[tx][ty] += (int)(this->attack_penalty*this->four_penalty*connect_cnt);
										else if(connect_cnt == 6)
											this->board_weight_attack[tx][ty] += INF;
										else
											this->board_weight_attack[tx][ty] += (int)(this->attack_penalty*connect_cnt);
									}else if(this->board[x][y] == this->op_color){
										if(connect_cnt >= 4)
											this->board_weight_defense[tx][ty] += (int)(this->four_penalty*connect_cnt);
										else if(connect_cnt == 6)
											this->board_weight_defense[tx][ty] += INF;
										else
											this->board_weight_defense[tx][ty] += connect_cnt;
									}
								}
							}		
						}
					}
				}
			}
}

int LK_AI_player::check_four(int _type){
	int four_cnt = 0;
	int visit[BOARD_SIZE][BOARD_SIZE][4] = {0};
	vector<pair<int,int> > stone;

	if(_type == this->my_color)
		stone = my_stone;
	else if(_type == this->op_color)
		stone = op_stone;

	int stone_cnt = stone.size();
	for(int i=0;i<stone_cnt;i++){
		int x = stone[i].first;
		int y = stone[i].second;
		for(int j=0;j<4;j++)
			for(int k=0;k<WINDOW;k++){
				vector<pair<int,int> > temp;
				int left = k, right = WINDOW - 1 - k;
				int cx = x, cy = y;
				while(left--){
					cx += dx[j];
					cy += dy[j];
					if(cx < 0 || cx >= BOARD_SIZE || cy < 0 || cy >= BOARD_SIZE)
						break;
					temp.push_back(make_pair(cx,cy));
				}
				cx = x; cy = y;
				while(right--){
					cx += dx[j+4];
					cy += dy[j+4];
					if(cx < 0 || cx >= BOARD_SIZE || cy < 0 || cy >= BOARD_SIZE)
						break;
					temp.push_back(make_pair(cx,cy));
				}
				if(temp.size() < WINDOW - 1)
					continue;
				bool active_threat = true;
				int connect_cnt = 1;
				for(int t=0;t<temp.size();t++){
					int tx = temp[t].first;
					int ty = temp[t].second;
					if(this->board[tx][ty] && this->board[tx][ty] != this->board[x][y])
						active_threat = false;
					if(this->board[tx][ty] == this->board[x][y])
						connect_cnt ++;
				}
				bool check_new_four = true;
				if(active_threat && connect_cnt >= 4){
					for(int t=0;t<temp.size();t++){
						int tx = temp[t].first;
						int ty = temp[t].second;
						if(visit[tx][ty][j])
							check_new_four = false;
						visit[tx][ty][j] = 1;
					}
					if(check_new_four)
						four_cnt ++;
					}
			}
	}
	return four_cnt;
}

pair< pair<int, int>, pair<int, int> > LK_AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){
	int x1_reversed, y1_reversed, x2_reversed, y2_reversed;
	int result_x[2];
	int result_y[2];
	pair< pair<int, int>, pair<int, int> > result;

	//reversed_input
	// save opponent's move
	if(!(this->my_color == WHITE && this->turn == 0)){
		x1_reversed = last_op_move.first.first;
		y1_reversed = BOARD_SIZE - last_op_move.first.second - 1;
		x2_reversed = last_op_move.second.first;
		y2_reversed = BOARD_SIZE - last_op_move.second.second - 1;

		new_stone(this->op_color, x1_reversed, y1_reversed);
		new_stone(this->op_color, x2_reversed, y2_reversed);
	}

	for(int num_move = 2; num_move > 0 ; num_move--){
		/*for(int i=0;i<BOARD_SIZE;i++){
			for(int j=0;j<BOARD_SIZE;j++)
				printf("%2d ",this->board_weight_attack[i][j] + this->board_weight_defense[i][j]);
			printf("\n");
		}printf("\n");*/
		pair<int,pair<int,int> > new_x_y = best_move(num_move);
		int x = new_x_y.second.first;
		int y = new_x_y.second.second;
		if(x == -1 && y == -1){
			for(int i=0;i<BOARD_SIZE;i++)
				for(int j=0;j<BOARD_SIZE;j++)
					if(!this->board[i][j]){
						x = i; y = j;
					}
		}
		new_stone(this->my_color,x,y);
		result_x[num_move - 1] = x;
		result_y[num_move - 1] = y;
		//printf("AI move: %d %d\n",x,y);
	}

	//reversed_output
	result.first.first = result_x[0];
	result.first.second = BOARD_SIZE - result_y[0] - 1;
	result.second.first = result_x[1];
	result.second.second = BOARD_SIZE - result_y[1] - 1;

	// save player's move
	this->turn++;
	return result;
}