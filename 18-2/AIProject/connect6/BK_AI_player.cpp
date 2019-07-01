#include <utility>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "AI_player.h"
#include "BK_AI_player.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

#ifndef __BK_SETTING__
#define __BK_SETTING__
#define WINDOW_SIZE 6
#endif

void BK_AI_player::init_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->board[i][j] = EMPTY;
		}
	}

	return;	
}

void BK_AI_player::init_active_board(){
	for(int i = 0; i<BOARD_SIZE; i++){
		for(int j = 0; j<BOARD_SIZE; j++){
			this->active_board[i][j] = EMPTY;
		}
	}

	return;
}

void BK_AI_player::init_player(int my_color, int op_color){
    //cout << "BK_init_player IN" << endl;

	struct Window threat;

	this->my_color = my_color;
	this->op_color = op_color;
	this->turn = 0;
	init_board();
	init_active_board();

    threat.count = 0;
    
    // vertical threats
    threat.type = 0;
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        threat.end_y = i;
        for( int j = WINDOW_SIZE - 1; j < BOARD_SIZE; j++ ) {
            threat.end_x = j;
            this->deactivate.push_back( threat );
        }
    }

    // horizontal threats
    threat.type = 1;
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        threat.end_x = i;
        for( int j = WINDOW_SIZE - 1; j < BOARD_SIZE; j++ ) {
            threat.end_y = j;
            this->deactivate.push_back( threat );
        }
    }

    // negative diagonal threats
    threat.type = 2;
    for( int i = WINDOW_SIZE - 1; i < BOARD_SIZE; i++ ) {
        threat.end_y = i;
        for( int j = WINDOW_SIZE - 1; j < BOARD_SIZE; j++ ) {
            threat.end_x = j;
            this->deactivate.push_back( threat );
        }
    }

    // positive diagonal threats
    threat.type = 3;
    for( int i = 0; i < BOARD_SIZE - 5; i++ ) {
        threat.end_y = i;
        for( int j = WINDOW_SIZE - 1; j < BOARD_SIZE; j++ ) {
            threat.end_x = j;
            this->deactivate.push_back( threat );
        }
    }

    //cout << "BK_init_player OUT" << endl;
	return;
}

void BK_AI_player::updateThreat( int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete, int _x, int _y ) {
	//cout << "BK_updateThreat IN" << endl;
    struct Window threat;
    _active_board[_x][_y] = -1;
    int diff_x, diff_y;
    int idx_length = _update.size();
    //cout << "Update" << endl;
    for( int idx = 0; idx < idx_length; idx++ ) {
        diff_x = _update[idx].end_x - _x;
        diff_y = _update[idx].end_y - _y;
        if( !_update[idx].type ) {
            if( !diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 )
                    _update[idx].count++;
                else if( diff_x == -1 || diff_x == 6 ) {
                    _update[idx] = _update.back();
                    _update.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _update[idx].type == 1 ) {
            if( !diff_x ) {
                if( diff_y >= 0 && diff_y <= 5 ) _update[idx].count++;
                else if( diff_y == -1 || diff_y == 6 ) {
                    _update[idx] = _update.back();
                    _update.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _update[idx].type == 2 ) {
            if( diff_x == diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 )
                    _update[idx].count++;
                else if( diff_x == -1 || diff_x == 6 ) {
                    _update[idx] = _update.back();
                    _update.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _update[idx].type == 3 ) {
            if( diff_x == diff_y * (-1) ) {
                if( diff_x >= 0 && diff_x <= 5 )
                    _update[idx].count++;
                else if( diff_x == -1 || diff_x == 6 ) {
                    _update[idx] = _update.back();
                    _update.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        }
    }

    //cout << "Activate" << endl;
    idx_length = _deactivate.size();
    for( int idx = 0; idx < idx_length; idx++ ) {
        diff_x = _deactivate[idx].end_x - _x;
        diff_y = _deactivate[idx].end_y - _y;
        // vertical
        if( !_deactivate[idx].type ) {
            if( !diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    threat = _deactivate[idx];
                    threat.count++;
                    // update active board
                    for( int i = threat.end_x - 5; i <= threat.end_x; i++ ) {
                        if( i >= 0 && i < BOARD_SIZE ) {
                            if( _active_board[i][_y] != -1 && _active_board[i][_y] != 1 ) {
                                _active_board[i][_y] = 1;
                            }
                        }
                    }
                    _update.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                } else if( diff_x == -1 || diff_x == 6 ) {
                    threat = _deactivate[idx];
                    _delete.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        }
        // horizontal
        else if( _deactivate[idx].type == 1 ) {
            if(!diff_x) {
                if( diff_y >= 0 && diff_y <= 5 ) {
                    threat = _deactivate[idx];
                    threat.count++;
                    // update active board
                    for( int i = threat.end_y - 5; i <= threat.end_y; i++ ) {
                        if( i >= 0 && i < BOARD_SIZE ) {
                            if( _active_board[_x][i] != -1 && _active_board[_x][i] != 1 ) {
                                _active_board[_x][i] = 1;
                            }
                        }
                    }
                    _update.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                } else if( diff_y == -1 || diff_y == 6 ) {
                    threat = _deactivate[idx];
                    _delete.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _deactivate[idx].type == 2 ) {
            if( diff_x == diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    threat = _deactivate[idx];
                    threat.count++;
                    // update active board
                    int dx = threat.end_x;
                    int dy = threat.end_y;
                    for( int i = 0; i< 6; i++ ) {
                        if( dx >= 0 && dx < BOARD_SIZE && dy >= 0 && dy < BOARD_SIZE )
                            if( _active_board[dx][dy] != -1 && _active_board[dx][dy] != 1 )
                                _active_board[dx][dy] = 1;
                        
                        dx--; dy--;               
                    }
                    _update.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                } else if( diff_x == -1 || diff_x == 6 ) {
                    threat = _deactivate[idx];
                    _delete.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _deactivate[idx].type == 3 ) {
            if( diff_x == diff_y * (-1) ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    threat = _deactivate[idx];
                    threat.count++;
                    // update active board
                    int dx = threat.end_x;
                    int dy = threat.end_y;
                    for( int i = 0; i < 6; i++ ) {
                        if( dx >= 0 && dx < BOARD_SIZE && dy >= 0 && dy < BOARD_SIZE )
                            if( _active_board[dx][dy] != -1 && _active_board[dx][dy] != 1 )
                                if( _active_board[dx][dy] != -1 )
                                    _active_board[dx][dy] = 1;
                        dx--; dy++;
                    }
                    _update.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                } else if( diff_x == -1 || diff_x == 6 ) {
                    threat = _deactivate[idx];
                    _delete.push_back(threat);
                    _deactivate[idx] = _deactivate.back();
                    _deactivate.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        }
    }

    //cout << "delete" << endl;
    idx_length = _delete.size();
    for( int idx = 0; idx < idx_length; idx++ ) {
        diff_x = _delete[idx].end_x - _x;
        diff_y = _delete[idx].end_y - _y;
        if( !_delete[idx].type ) {
            if( !diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    _delete[idx] = _delete.back();
                    _delete.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _delete[idx].type == 1 ) {
            if(!diff_x) {
                if( diff_y >= 0 && diff_y <= 5 ) {
                    _delete[idx] = _delete.back();
                    _delete.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _delete[idx].type == 2 ) {
            if( diff_x == diff_y ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    _delete[idx] = _delete.back();
                    _delete.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        } else if( _delete[idx].type == 3 ) {
            if( diff_x == diff_y * (-1) ) {
                if( diff_x >= 0 && diff_x <= 5 ) {
                    _delete[idx] = _delete.back();
                    _delete.pop_back();
                    idx_length--;
                    idx--;
                }
            }
        }
    }
    //cout << "BK_updateThreat OUT" << endl;
}

void BK_AI_player::firstSearch( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete ){
	//cout << "BK_firstSearch IN" << endl;
    int score;
    vector< Window > first_my = _update;
    vector< Window > first_op = _delete;
    vector< Window > first_deact = _deactivate;
    int first_active_board[BOARD_SIZE][BOARD_SIZE];
    for( int i = 0; i < BOARD_SIZE; i++ )
        for( int j = 0; j < BOARD_SIZE; j++ )
            first_active_board[i][j] = _active_board[i][j];
    
    updateThreat( first_active_board, first_deact, first_my, first_op, _x, _y );

    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            if( first_active_board[i][j] == 1 && first_active_board[i][j] != -1 ) {
                score = this->next_stone.score;
                secondSearch( i, j, first_active_board, first_deact, first_my, first_op );
                if( score != this->next_stone.score ) {
                    this->next_stone.first_x = _x;
                    this->next_stone.first_y = _y;
                   // printf( "Search: (%d %d), (%d %d)\n",\
                        this->next_stone.first_x + 1, this->next_stone.first_y + 1,\
                        this->next_stone.second_x, this->next_stone.second_y );
                    //printf( "score: %d\n\n", this->next_stone.score );

                }
            }
        }
    }
    //cout << "BK firstSearch OUT" << endl;
}

void BK_AI_player::secondSearch( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete ){
	//cout << "BK_secondSearch IN" << endl;
    //cout << "1" << endl;
    vector< Window > _my;
    int len = _update.size();
    for( int i = 0; i < len; i++ )
        _my.push_back( _update[i] );
    
    //cout << "2" << endl;
    vector< Window > _op;
    //cout << _delete.size() << endl;
    len = _delete.size();
    //cout << "push_back" << endl;
    for( int i = 0; i < len; i++ ){
        _op.push_back( _delete[i] );
        //cout << "t" << endl;
    }
    
    //cout << "3" << endl;
    vector< Window > _deact;
    len = _deactivate.size();
    for( int i = 0; i < len; i++ )
        _deact.push_back( _deactivate[i] );
    
    //cout << "4" << endl;
    int second_active_board[BOARD_SIZE][BOARD_SIZE];
    for( int i = 0; i < BOARD_SIZE; i++ )
        for( int j = 0; j < BOARD_SIZE; j++ )
            second_active_board[i][j] = _active_board[i][j];
    //cout << "update threats" << endl;
    updateThreat( second_active_board, _deact, _my, _op, _x, _y );

    //cout << "scoring" << endl;
    int score = 0;
    len = _my.size();
    for( int idx = 0; idx < len; idx++ ) {
        if( _my[idx].count == 0 ) score += 2;
        else if( _my[idx].count == 1 ) score += 4;
        else if( _my[idx].count == 2 ) score += 77;
        else if( _my[idx].count == 3 ) score += 107;
        else if( _my[idx].count == 4 ) score += 512;
        else if( _my[idx].count == 5 ) score += 512;
        else if( _my[idx].count == 6 ) score += 99999;
    }

    len = _op.size();
    for( int idx = 0; idx < len; idx++ ) {
        if( _op[idx].count == 0 ) score -= 16;
        else if( _op[idx].count == 1 ) score -= 64;
        else if( _op[idx].count == 2 ) score -= 257;
        else if( _op[idx].count == 3 ) score -= 385;
        else if( _op[idx].count == 4 ) score -= 9999;
        else if( _op[idx].count == 5 ) score -= 9999;
        else if( _op[idx].count == 6 ) score -= 99999;
    }

    if( this->next_stone.score < score ) {
        this->next_stone.score = score;
        this->next_stone.second_x = _x;
        this->next_stone.second_y = _y;
    }
    //cout << "BK_secondSearch OUT" << endl;
}

void BK_AI_player::set_forced_move(int color, int x, int y){
	//cout << "BK_set_forced_move IN" << endl;
    int x_reversed, y_reversed;

    x_reversed = BOARD_SIZE - y -1;
    y_reversed = x;

	this->board[x_reversed][y_reversed] = color;

	if(this->my_color == color) updateThreat( this->active_board, this->deactivate, this->cur_my, this->cur_op, x_reversed, y_reversed );
	else updateThreat( this->active_board, this->deactivate, this->cur_op, this->cur_my, x_reversed, y_reversed );
	
    //cout << "BK_set_forced_move OUT" << endl;
    return;
}

pair< pair<int, int>, pair<int, int> > BK_AI_player::return_move(pair< pair<int, int>, pair<int, int> > last_op_move){
	//cout << "BK_return_move IN" << endl;
    int x1_reversed, y1_reversed, x2_reversed, y2_reversed;
	pair< pair<int, int>, pair<int, int> > result;

	/* convert 
				(18,18)
								to			?
		(0,0)
	*/
    x1_reversed = BOARD_SIZE - last_op_move.first.second - 1;
	y1_reversed = last_op_move.first.first;
	x2_reversed = BOARD_SIZE - last_op_move.second.second - 1;
	y2_reversed = last_op_move.second.first;


	// save opponent's move
	if(!(this->my_color == WHITE && this->turn == 0)){
		this->board[ x1_reversed ][ y1_reversed ] = this->op_color;
		updateThreat(this->active_board, this->deactivate, this->cur_op, this->cur_my, x1_reversed, y1_reversed);
		this->board[ x2_reversed ][ y2_reversed ] = this->op_color;
		updateThreat(this->active_board, this->deactivate, this->cur_op, this->cur_my, x2_reversed, y2_reversed);
	}

	this->next_stone.score = -99999;
	for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            if( this->active_board[i][j] == 1 && this->active_board[i][j] != -1 ) {
                firstSearch( i, j, this->active_board, this->deactivate, this->cur_my, this->cur_op );
            }
        }
    }

    this->board[this->next_stone.first_x][this->next_stone.first_y] = this->my_color;
    updateThreat( this->active_board, this->deactivate, this->cur_my, this->cur_op, this->next_stone.first_x, this->next_stone.first_y );
    this->board[this->next_stone.second_x][this->next_stone.second_y] = this->my_color;
    updateThreat( this->active_board, this->deactivate, this->cur_my, this->cur_op, this->next_stone.second_x, this->next_stone.second_y );

    /*
    should be [9][10]

    			(18,18)

			(x1_reversed, y1_reversed)

    	(0,0)
    */

    x1_reversed = this->next_stone.first_y;
    y1_reversed = BOARD_SIZE - this->next_stone.first_x - 1;
    x2_reversed = this->next_stone.second_y;
    y2_reversed = BOARD_SIZE - this->next_stone.second_x - 1;

    result.first.first = x1_reversed; 
    result.first.second = y1_reversed;
    result.second.first = x2_reversed;
    result.second.second = y2_reversed;

	// save player's move
	this->turn++;
    //cout << "BK_return_move OUT" << endl;
	return result;
}