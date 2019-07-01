#include <utility>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include "AI_player.h"
#include "UTT.h"
using namespace std;

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19 // from 0 to 18
#define MAX_TURN 180
#endif

bool _compare( struct Pos a, struct Pos b){
	return a.score > b.score;
}

void UTT_player::initBoard() {
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            this->board[i][j] = EMPTY;
            this->active_board[i][j] = EMPTY;
        }
    }

    return;
}

void UTT_player::initThreat() {
    int tmp;

    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            for( int i = 0; i < 7; i++ ) {
                this->my_threat_board[x][y][i] = 0;
                this->op_threat_board[x][y][i] = 0;
            }
        }
    }

    for( int i = 0; i < 6; i++ ) {
        this->my_threat[i] = 0;
        this->op_threat[i] = 0;
    }

    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            // vertical threat
            if( (x >= 5) && (x <= 13) )
                this->my_threat_board[x][y][0] += 6;
            else if( x < 5 )
                this->my_threat_board[x][y][0] += (x + 1);
            else
                this->my_threat_board[x][y][0] += (BOARD_SIZE - x);
            
            // horizontal threat
            if( (y >=5) && (y <= 13) )
                this->my_threat_board[x][y][0] += 6;
            else if( y < 5 )
                this->my_threat_board[x][y][0] += (y + 1);
            else
                this->my_threat_board[x][y][0] += (BOARD_SIZE - y);
            
            // positive linear
            tmp = 0;
            for( int z = 0; z < 6; z++ ) {
				if( (x - z >= 0 && y + z < BOARD_SIZE) && (x - z + 5 < BOARD_SIZE && y + z - 5 >= 0) ) {
					tmp++;
				}
			}
            this->my_threat_board[x][y][0] += tmp;

            // negative linear
            tmp = 0;
			for( int z = 0; z < 6; z++ ){
				if( (x - z >= 0 && y - z >= 0) && (x - z + 5 < BOARD_SIZE && y - z + 5 < BOARD_SIZE) ) {
					tmp++;
				}
			}
			this->my_threat_board[x][y][0] += tmp;

            this->op_threat_board[x][y][0] = this->my_threat_board[x][y][0];
        }
    }

    // total threat
    for( int i = 0; i < 7; i++ ) {
        this->my_threat[i] = 0;
        this->op_threat[i] = 0;
    }
    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            this->my_threat[0] += this->my_threat_board[x][y][0];
            this->op_threat[0] += this->op_threat_board[x][y][0];
        }
    }


    return;
}

void UTT_player::init_player( int _my_color, int _op_color ) {
    this->my_color = _my_color;
    this->op_color = _op_color;
    this->turn = 0;
    initThreat();
    initBoard();
    //updateActiveBoard( 9, 9, WHITE, this->active_board );
    //updateThreat( 9, 9, WHITE, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
    //updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );
    //printBoard( this->score_board );

    return;
}

pair< pair<int, int>, pair<int, int> > UTT_player::return_fixed_start_move(){
	pair< pair<int, int>, pair<int, int> > result;
	result.first.first = BOARD_SIZE/2 - 2;
	result.first.second = BOARD_SIZE/2;
	result.second.first = BOARD_SIZE/2 - 1;
	result.second.second = BOARD_SIZE/2 + 1;

	return result;
}

void UTT_player::set_forced_move( int _x, int _y, int _color ) {
    if( _color == this->my_color ) {
        updateActiveBoard( _x, _y, _color, this->active_board );
        updateThreat( _x, _y, _color, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
        updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );
    } else {
        updateActiveBoard( _x, _y, _color, this->active_board );
        updateThreat( _x, _y, _color, this->board, this->op_threat_board, this->my_threat_board, this->op_threat, this->my_threat );
        updateScoreBoard( this->score_board, this->active_board, this->op_threat_board, this->op_threat_board );
    }

    return;
}

void UTT_player::updateActiveBoard( int _x, int _y, int _color, int _active_board[][BOARD_SIZE] ) {
    _active_board[_x][_y] = -1;
    printf( "in upact: (%d %d)\n", _x, _y );
    for( int x = _x - 5; x <= _x + 5; x++ ) {
        if( (x < 0) || (x >= BOARD_SIZE) ) continue;
        if( _active_board[x][_y] ) continue;
        _active_board[x][_y] = _color;
    }

    for( int y = _y - 5; y <= _y + 5; y++ ) {
        if( (y < 0) || ( y >= BOARD_SIZE) ) continue;
        if( _active_board[_x][y] ) continue;
        _active_board[_x][y] = _color;
    }

    for( int i = -5; i < 6; i++ ) {
        if( ( (_x+i) < 0 ) || ( (_x+i) >= BOARD_SIZE ) || ( (_y+i) < 0 ) || ( (_y+i) >= BOARD_SIZE ) ) continue;
        if( _active_board[_x+i][_y+i] ) continue;
        _active_board[_x+i][_y+i] = _color;
    }

    for( int i = -5; i < 6; i++ ) {
        if( ( (_x+i) < 0 ) || ( (_x+i) >= BOARD_SIZE ) || ( (_y-i) < 0 ) || ( (_y-i) >= BOARD_SIZE ) ) continue;
        if( _active_board[_x+i][_y-i] ) continue;
        _active_board[_x+i][_y-i] = _color;
    }

    return;
}

void UTT_player::updateThreat ( int _x, int _y, int _color, int _board[][BOARD_SIZE], int _my_threat_board[][BOARD_SIZE][7],\
                                int _op_threat_board[][BOARD_SIZE][7], int _my_threat[7], int _op_threat[7] ) {
    int cur_my_cnt, cur_op_cnt, cur_my_color, cur_op_color, tmp, b;
    int (*cur_my_threat_cnt)[BOARD_SIZE][7];
    int (*cur_op_threat_cnt)[BOARD_SIZE][7];

    if( _color == this->my_color ) {
        cur_my_threat_cnt = _my_threat_board;
        cur_op_threat_cnt = _op_threat_board;
        cur_my_color = this->my_color;
        cur_op_color = this->op_color;
    } else {
        cur_my_threat_cnt = _op_threat_board;
        cur_op_threat_cnt = _my_threat_board;
        cur_my_color = this->op_color;
        cur_op_color = this->my_color;
    }
    
    if( _board[_x][_y] != EMPTY ) {
        cout << "Overlap" << endl;
        return;
    }

    // ===================horizontal threat===================
    for( int i = ( (_x - 5 > 0) ? (_x - 5) : 0 ); i <= _x; i++ ) {
        if( i + 5 >= BOARD_SIZE ) continue;

        // check my threat
        cur_my_cnt = 0;
        if( ( i - 1 < 0 || _board[i-1][_y] != cur_my_color ) && ( i + 6 >= BOARD_SIZE || _board[i+6][_y] != cur_my_color ) ) {
            for( b = 0; b < 6; b++ ) {
                if( _board[i+b][_y] == cur_my_color ) cur_my_cnt++;
                else if( _board[i+b][_y] == cur_op_color ) {
                    cur_my_cnt = -1;
                    break;
                }
            }

            if( cur_my_cnt != -1 ) {
                for( b = 0; b < 6; b++ ) {
                    if( _board[i+b][_y] == EMPTY ) {
                        cur_my_threat_cnt[i+b][_y][cur_my_cnt] -= 1;
                        cur_my_threat_cnt[i+b][_y][cur_my_cnt+1] += 1;
                    }
                }
            }
        }

        // check op threat
        cur_op_cnt = 0;
        if( ( i - 1 < 0 || _board[i-1][_y] != cur_op_color ) && ( i + 6 >= BOARD_SIZE || _board[i+6][_y] != cur_op_color ) ) {
            for( b = 0; b < 6; b++ ) {
                if( _board[i+b][_y] == cur_op_color ) cur_op_cnt++;
                else if( _board[i+b][_y] == cur_my_color ) {
                    cur_op_cnt -= 1;
                    break;
                }
            }

            if( cur_op_cnt != -1 ) {
                for( b = 0; b < 6; b++ ) {
                    if( _board[i+b][_y] == EMPTY ) {
                        cur_op_threat_cnt[i+b][_y][cur_op_cnt] -= 1;
                    }
                }
            }
        }
    }

    // ===================vertical threat===================
    for( int j = ( (_y - 5 > 0) ?  _y - 5 : 0 ); j <= _y; j++ ) {
        if( j + 5 >= BOARD_SIZE ) continue;

        // check my threat
        cur_my_cnt = 0;
        if( ( j - 1 < 0 || _board[_x][j-1] != cur_my_color ) && ( j + 6 >= BOARD_SIZE || _board[_x][j+6] != cur_my_color ) ) {
			for( b = 0; b < 6; b++ ) {
				if( _board[_x][j+b] == cur_my_color ) cur_my_cnt++;
				else if( _board[_x][j+b] == cur_op_color){
					cur_my_cnt = -1;
					break;
				}
			}

			if( cur_my_cnt != -1 ) {
				for( b = 0; b < 6; b++ ) {
					if( _board[_x][j+b] == EMPTY ) {
						cur_my_threat_cnt[_x][j+b][cur_my_cnt] -= 1;
						cur_my_threat_cnt[_x][j+b][cur_my_cnt+1] += 1;
					}
				}
			}
		}

        // check op threat
        cur_op_cnt = 0;
		if( ( j - 1 < 0 || _board[_x][j-1] != cur_op_color ) && ( j + 6 >= BOARD_SIZE || _board[_x][j+6] != cur_op_color ) ) {
			for( b = 0; b < 6; b++ ) {
				if( _board[_x][j+b] == cur_op_color ) cur_op_cnt++;
				else if( _board[_x][j+b] == cur_my_color ) {
					cur_op_cnt = -1;
					break;
				}
			}

			if( cur_op_cnt != -1){
				for( b = 0; b < 6; b++ ) {
					if( _board[_x][j+b] == EMPTY ) {
						cur_op_threat_cnt[_x][j+b][cur_op_cnt] -= 1;
					}
				}
			}
		} 
	}

    // ===================negative linear threat===================
    tmp = ( BOARD_SIZE - _y - 1 < _x ) ? (BOARD_SIZE - _y - 1) : _x;
    if( tmp > 5 ) tmp = 5;
    for( int i = 0; i <= tmp; i++ ) {
        if( ( _x - i + 5 >= BOARD_SIZE ) || ( _y + i - 5 < 0 ) ) continue;

        // check my threat
        cur_my_cnt = 0;
        if( ( (_x - i - 1) < 0 || (_y + i + 1) >= BOARD_SIZE || _board[_x-i-1][_y+1] != cur_my_color ) &&\
            ( (_x - i + 6) >= BOARD_SIZE || (_y + i - 6) < 0 || _board[_x-i+6][_y+i-6] != cur_my_color) ) {
            for( b = 0; b < 6; b++ ) {
				if( _board[_x-i+b][_y+i-b] == cur_my_color ) cur_my_cnt++;
				else if( _board[_x-i+b][_y+i-b] == cur_op_color ) {
					cur_my_cnt = -1;
					break;
				}
			}

			if( cur_my_cnt != -1 ) {
				for( b = 0; b < 6; b++ ) {
					if( _board[_x-i+b][_y+i-b] == EMPTY ) {
						cur_my_threat_cnt[_x-i+b][_y+i-b][cur_my_cnt] -= 1;
						cur_my_threat_cnt[_x-i+b][_y+i-b][cur_my_cnt+1] += 1;
					}
				}
			}
		}

        // check op threat
        cur_op_cnt = 0;
		if( ( (_x - i - 1) < 0 || (_y + i + 1) >= BOARD_SIZE || _board[_x-i-1][_y+i+1] != cur_op_color ) &&\
            ( (_x - i + 6) >= BOARD_SIZE || (_y + i - 6) < 0 || _board[_x-i+6][_y+i-6] != cur_op_color) ) {
			for( b= 0; b < 6; b++ ) {
				if( _board[_x-i+b][_y+i-b] == cur_op_color) cur_op_cnt++;
				else if( _board[_x-i+b][_y+i-b] == cur_my_color ) {
					cur_op_cnt = -1;
					break;
				}
			}

			if( cur_op_cnt != -1 ) {
				for( b = 0; b < 6; b++ ) {
					if( _board[_x-i+b][_y+i-b] == EMPTY ) {
						cur_op_threat_cnt[_x-i+b][_y+i-b][cur_op_cnt] -= 1;
					}
				}
			}
		} 
	}

    // ===================positive linear threat===================
    tmp = ( _x < _y ) ? _x : _y;
	if ( tmp > 5 ) tmp = 5;
	for( int i = 0; i <= tmp; i++ ) {
		if( (_x - i + 5 >= BOARD_SIZE) || (_y - i + 5 >= BOARD_SIZE) ) continue;

		// check my live line first
		cur_my_cnt = 0;
		if( ( (_x - i - 1) < 0 || (_y - i - 1) < 0 || _board[_x-i-1][_y-i-1] != cur_my_color ) &&\
            ( (_x - i + 6) >= BOARD_SIZE || (_y - i + 6) >= BOARD_SIZE || _board[_x-i+6][_y-i+6] != cur_my_color) ) {
			for( b = 0; b < 6; b++ ) {
				if( _board[_x-i+b][_y-i+b] == cur_my_color) cur_my_cnt++;
				else if( _board[_x-i+b][_y-i+b] == cur_op_color ) {
					cur_my_cnt = -1;
					break;
				}
			}

			if( cur_my_cnt != -1 ) {
				for( b = 0; b < 6; b++ ) {
					if( _board[_x-i+b][_y-i+b] == EMPTY ) {
						cur_my_threat_cnt[_x-i+b][_y-i+b][cur_my_cnt] -= 1;
						cur_my_threat_cnt[_x-i+b][_y-i+b][cur_my_cnt+1] += 1;
					}
				}
			}
		} 

		// check op live line first
		cur_op_cnt = 0;
		if( ( (_x - i - 1) < 0 || (_y - i - 1) < 0 || _board[_x-i-1][_y-i-1] != cur_op_color ) &&\
            ( (_x - i + 6) >= BOARD_SIZE || (_y - i + 6)  >= BOARD_SIZE || _board[_x-i+6][_y-i+6] != cur_op_color) ) {
			for( b = 0; b < 6; b++ ) {
				if( _board[_x-i+b][_y-i+b] == cur_op_color ) cur_op_cnt++;
				else if( _board[_x-i+b][_y-i+b] == cur_my_color ) {
					cur_op_cnt = -1;
					break;
				}
			}

			if( cur_op_cnt != -1 ) {
				for( b = 0; b < 6; b++ ) {
					if( _board[_x-i+b][_y-i+b] == EMPTY ) {
						cur_op_threat_cnt[_x-i+b][_y-i+b][cur_op_cnt] -= 1;
					}
				}
			}
		} 
	}

    // if move make exist live line to over 6 -> remove exist live line
	// left hor x-1,y ~ x-6
	cur_my_cnt = 0;
	if( (_x - 6 >= 0) && (_x - 7 < 0 || _board[_x-7][_y]!=cur_my_color) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x-b][_y] == cur_my_color) cur_my_cnt++;
			else if( _board[_x-b][_y] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x-b][_y] == EMPTY ) {
					cur_my_threat_cnt[_x-b][_y][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//right hor x+1,y ~ x+6,y
	cur_my_cnt = 0;
	if( (_x + 6 < BOARD_SIZE) && (_x+7 >= BOARD_SIZE || _board[_x+7][_y] != cur_my_color) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x+b][_y] == cur_my_color) cur_my_cnt++;
			else if( _board[_x+b][_y] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x+b][_y] == EMPTY ) {
					cur_my_threat_cnt[_x+b][_y][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//upside ver x,y+1 ~ x, y+6
	cur_my_cnt = 0;
	if( (_y + 6 < BOARD_SIZE) && (_y + 7 >= BOARD_SIZE || _board[_x][_y+7] != cur_my_color) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x][_y+b] == cur_my_color) cur_my_cnt++;
			else if( _board[_x][_y+b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x][_y+b] == EMPTY ) {
					cur_my_threat_cnt[_x][_y+b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//downside ver x,y-1 ~ x, y-6
	cur_my_cnt = 0;
	if( (_y - 6 >= 0) && (_y - 7 < 0 || _board[_x][_y-7] != cur_my_color ) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x][_y-b] == cur_my_color) cur_my_cnt++;
			else if( _board[_x][_y-b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x][_y-b] == EMPTY ) {
					cur_my_threat_cnt[_x][_y-b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//upside top left down right x-1,y+1 ~ x-6,y+6
	cur_my_cnt = 0;
	if( (_x - 6 >= 0 && _y + 6 < BOARD_SIZE) && (_x - 7 < 0 || _y + 7 >= BOARD_SIZE || _board[_x-7][_y+7] != cur_my_color ) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x-b][_y+b] == cur_my_color) cur_my_cnt++;
			else if( _board[_x-b][_y+b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x-b][_y+b] == EMPTY ) {
					cur_my_threat_cnt[_x-b][_y+b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//downside top left down right x+1,y-1 ~ x+6,y-6
	cur_my_cnt = 0;
	if( (_x + 6 < BOARD_SIZE && _y - 6 >= 0) && (_x + 7 >= BOARD_SIZE || _y-7 < 0 || _board[_x+7][_y-7] != cur_my_color ) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x+b][_y-b] == cur_my_color ) cur_my_cnt++;
			else if( _board[_x+b][_y-b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x+b][_y-b] == EMPTY ) {
					cur_my_threat_cnt[_x+b][_y-b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//upside top right down left x+1,y+1 ~ x+6,y+6
	cur_my_cnt = 0;
	if( (_x + 6 < BOARD_SIZE && _y + 6 < BOARD_SIZE) && (_x + 7 >= BOARD_SIZE || _y + 7 >= BOARD_SIZE || _board[_x+7][_y+7] != cur_my_color ) ) {
		for( b = 1; b <= 6; b++ ) {
			if( _board[_x+b][_y+b] == cur_my_color ) cur_my_cnt++;
			else if( _board[_x+b][_y+b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x+b][_y+b] == EMPTY ) {
					cur_my_threat_cnt[_x+b][_y+b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	//downside top right down left x-1,y-1 ~ x-6,y-6
	cur_my_cnt = 0;
	if( (_x - 6 >= 0 && _y - 6 >= 0) && (_x - 7 < 0 || _y - 7 < 0 || _board[_x-7][_y-7] != cur_my_color ) ) {
		for( b = 1; b <= 6; b++ ) {
			if(_board[_x-b][_y-b] == cur_my_color ) cur_my_cnt++;
			else if( _board[_x-b][_y-b] == cur_op_color ) {
				cur_my_cnt = -1;
				break;
			}
		}

		if( cur_my_cnt != -1 ) {
			for( b = 1; b <= 6; b++ ) {
				if( _board[_x-b][_y-b] == EMPTY ) {
					cur_my_threat_cnt[_x-b][_y-b][cur_my_cnt] -= 1;
				}
			}
		}
	}

	// set move
	_board[_x][_y] = cur_my_color;
    printf( "(_x, _y) in upthreat: (%d %d)\n", _x, _y );

    // calculate all threats
    for( int i = 0; i < 7; i++ ) {
        _my_threat[i] = 0;
        _op_threat[i] = 0;
    }

    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            for( int i = 0; i < 7; i++ ) {
                _my_threat[i] += _my_threat_board[x][y][i];
                _op_threat[i] += _op_threat_board[x][y][i];
            }
        }
    }

	return;
}

void UTT_player::updateScoreBoard( int _score_board[][BOARD_SIZE], int _active_board[][BOARD_SIZE],\
                                int _my_threat_board[][BOARD_SIZE][7], int _op_threat_board[][BOARD_SIZE][7] ) {
    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            if( _active_board[x][y] == -1 ) {
                _score_board[x][y] = -1;
                continue;
            }
            _score_board[x][y] += _my_threat_board[x][y][1] * 16;
            _score_board[x][y] += _my_threat_board[x][y][2] * 192;
            _score_board[x][y] += _my_threat_board[x][y][3] * 192;
            _score_board[x][y] += _my_threat_board[x][y][4] * 999999;
            _score_board[x][y] += _my_threat_board[x][y][5] * 999999;
            _score_board[x][y] += _my_threat_board[x][y][6] * 999999;
        }
    }

    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            if( _score_board[x][y] == - 1 ) continue;
            _score_board[x][y] += _op_threat_board[x][y][1] * 16;
            _score_board[x][y] += _op_threat_board[x][y][2] * 192;
            _score_board[x][y] += _op_threat_board[x][y][3] * 192;
            _score_board[x][y] += _op_threat_board[x][y][4] * 99999;
            _score_board[x][y] += _op_threat_board[x][y][5] * 99999;
            _score_board[x][y] += _op_threat_board[x][y][6] * 999999;
        }
    }

    return;
}

pair< pair<int, int>, pair<int, int> > UTT_player::return_move( pair< pair<int, int>, pair<int, int> > last_op_move ) {
    pair< pair<int, int>, pair<int, int> > result;
    int x1 = last_op_move.first.first;
    int y1 = last_op_move.first.second;
    int x2 = last_op_move.second.first;
    int y2 = last_op_move.second.second;
    printf( "last_op_move: (%d %d), (%d %d)\n", x1, y1, x2, y2 );

    if(!(this->my_color == WHITE && this->turn == 0)){
        updateActiveBoard( x1, y1, WHITE, this->active_board );
        updateThreat( x1, y1, WHITE, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
        updateActiveBoard( x2, y2, WHITE, this->active_board );
        updateThreat( x2, y2, WHITE, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
        updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );
    }

    if(this->my_color == WHITE && this->turn == 0) result = return_fixed_start_move();
    else result = search( this->board, this->active_board, this->score_board );
/*
    if( this->turn == 0 )
        set_forced_move( 9, 9, BLACK );
    else {
        if( this->my_color == BLACK ) {
            updateActiveBoard( x1, y1, WHITE, this->active_board );
            updateThreat( x1, y1, WHITE, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
            updateActiveBoard( x2, y2, WHITE, this->active_board );
            updateThreat( x2, y2, WHITE, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
            updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );
        } else {
            updateActiveBoard( x1, y1, BLACK, this->active_board );
            updateThreat( x1, y1, BLACK, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
            updateActiveBoard( x2, y2, BLACK, this->active_board );
            updateThreat( x2, y2, BLACK, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
            updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );
        }
    }*/
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            printf("%d ", this->active_board[i][j] );
        }
        printf("\n");
    }
    cout << endl;
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            printf("%d ", this->score_board[i][j] );
        }
        printf("\n");
    }
    cout << endl;
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            printf("%d ", this->board[i][j] );
        }
        printf("\n");
    }
    
    

    //result = search( this->board, this->active_board, this->score_board );
    x1 = result.first.first;
    y1 = result.first.second;
    x2 = result.second.first;
    y2 = result.second.second;

    updateActiveBoard( x1, y1, this->my_color, this->active_board );
    updateThreat( x1, y1, this->my_color, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
    updateActiveBoard( x2, y2, this->my_color, this->active_board );
    updateThreat( x2, y2, this->my_color, this->board, this->my_threat_board, this->op_threat_board, this->my_threat, this->op_threat );
    updateScoreBoard( this->score_board, this->active_board, this->my_threat_board, this->op_threat_board );

    //printBoard( this->score_board );

    return result;
}

pair< pair<int, int>, pair<int, int> > UTT_player::search( int _board[][BOARD_SIZE], int _active_board[][BOARD_SIZE], int _score_board[][BOARD_SIZE] ) {
    pair< pair<int, int>, pair<int, int> > result;
    vector< Pos > cand;

    int new_active_board[BOARD_SIZE][BOARD_SIZE];
    memcpy( new_active_board, _active_board, sizeof(int) * BOARD_SIZE * BOARD_SIZE );

    struct Pos stone = { -1, -1, -1000 };
    cand.push_back( stone );
    for( int x = 0; x < BOARD_SIZE; x++ ) {
        for( int y = 0; y < BOARD_SIZE; y++ ) {
            if( new_active_board[x][y] == -1 || new_active_board[x][y] == 0 ) continue;
            if( (_board[x][y] == EMPTY) && (cand.back().score < _score_board[x][y] ) ) {
                if( cand.size() < 5 ) {
                    stone.x = x;
                    stone.y = y;
                    stone.score = _score_board[x][y];
                    cand.push_back( stone );
                } else {
                    cand.pop_back();
                    stone.x = x;
                    stone.y = y;
                    stone.score = _score_board[x][y];
                    cand.push_back( stone );
                }

                sort( cand.begin(), cand.end(), _compare );
            }
        }
    }

    result.first.first = cand[0].x;
    result.first.second = cand[0].y;
    result.second.first = cand[1].x;
    result.second.second = cand[1].y;
    printf( "(%d %d), (%d %d)\n", cand[0].x, cand[0].y, cand[1].x, cand[1].y );

    return result;
}

void UTT_player::printBoard( int _board[][BOARD_SIZE] ) {
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            printf( "%d ", _board[i][j] );
        }
        printf( "\n" );
    }
    return;
}
/*
int main() {
    UTT_player ai;

    ai.init_player( BLACK, WHITE );

    return 0;
}*/