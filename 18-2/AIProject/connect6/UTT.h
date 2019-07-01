#include <utility>
#include <tuple>
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

#ifndef UTT_H
#define UTT_H
#define WINDOW_SIZE 6

struct Pos {
    int x;
    int y;
    int score;
};

class UTT_player : public AI_player {
    protected:
        int active_board[BOARD_SIZE][BOARD_SIZE];
        int score_board[BOARD_SIZE][BOARD_SIZE];
        int my_threat_board[BOARD_SIZE][BOARD_SIZE][7];
        int op_threat_board[BOARD_SIZE][BOARD_SIZE][7];
        int my_threat[7];
        int op_threat[7];

        void initBoard();
        void initThreat();
        void updateActiveBoard( int _x, int _y, int _color, int _active_board[BOARD_SIZE][BOARD_SIZE] );
        void updateThreat( int _x, int _y, int _color, int _board[][BOARD_SIZE], int _my_threat_board[][BOARD_SIZE][7],\
                                int _op_threat_board[][BOARD_SIZE][7], int _my_threat[7], int _op_threat[7] );
        void updateScoreBoard( int _score_board[][BOARD_SIZE], int _active_board[][BOARD_SIZE],\
                                int _my_threat_board[][BOARD_SIZE][7], int _op_threat_board[][BOARD_SIZE][7] );
        pair< pair<int, int>, pair<int, int> > search( int _board[][BOARD_SIZE], int _active_board[][BOARD_SIZE], int _score_board[][BOARD_SIZE] );
        pair< pair<int, int>, pair<int, int> > return_fixed_start_move();

    public:
        void init_player( int _my_color, int _op_color );
        void set_forced_move( int _x, int _y, int _color );
        pair< pair<int, int>, pair<int, int> > return_move(pair< pair<int, int>, pair<int, int> > last_op_move);
        void printBoard( int _board[][BOARD_SIZE] );
};

#endif