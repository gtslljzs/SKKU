#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include "AI_player.h"
#include "BK_AI_player.h"
#include "utd.h"

#ifndef __SETTING__
#define __SETTING__
#define BLACK 1
#define WHITE 2
#define EMPTY 0
#define BOARD_SIZE 19
#define WINDOW_SIZE 6
#define MAX_TURN 180
#endif

#ifndef __UTD_SETTING__
#define __UTD_SETTING__
#endif

void AI::initBoard() {
    for( int i = 0; i < BOARD_SIZE; i++ ) {
        for( int j = 0; j < BOARD_SIZE; j++ ) {
            this->board[i][j] = EMPTY;
            this->active_board[i][j] = EMPTY;
            this->weight_board[i][j] = EMPTY;
        }
    }
    
    return;
}

void AI::init_player( int my_color, int op_color ) {
    struct Window threat;

    this->my_color = my_color;
    this->op_color = op_color;
    this->turn = 0;
    threat.count = 0;
    initBoard();

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
        for( int j = WINDOW_SIZE - 1; j< BOARD_SIZE; j++ ) {
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

    return;
}

void AI::update( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], int _weight_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete ) {
    // update weight board, active board and threats
    // weight board를 보고 한 수당 9개 후보에 대해 다음 내 턴까지 탐색
    // 배열에 9개 후보 저장 - weight board를 업데이트 할 때
    struct Window threat;
    _weight_board[_x][_y] = -1;