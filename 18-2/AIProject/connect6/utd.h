#include <utility>
#include <vector>
#include "AI_player.h"

#ifndef __SETTING__
#define __SETTING__
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BOARD_SIZE 19
#define WINDOW_SIZE 6
#define MAX_TURN 180
#endif

#ifndef UTD_H
#define UTD_H
#define WINDOW_SIZE 6

class AI : public AI_player {
    protected:
        int color;
        int active_board[BOARD_SIZE][BOARD_SIZE];
        int weight_board[BOARD_SIZE][BOARD_SIZE];
        struct Window {
            int type;
            int count;
            int end_x;
            int end_y;
        };
        struct Stone {
            int pos[4];
            double score;
        };
        vector< Window > deactivate;
        vector< Window > cur_black;
        vector< Window > cur_white;
        struct Stone next_stone;

        void initBoard();
        void update ( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], int _weight_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete );
        void search( int _x, int _y, int _active_board[BOARD_SIZE][BOARD_SIZE], int _weight_board[BOARD_SIZE][BOARD_SIZE], vector< Window >& _deactivate, vector< Window >& _update, vector< Window >& _delete, int cnt );

    public:
        void init_player( int my_color, int op_color );
        void set_forced_move( int color, int x, int y );
        pair< pair< int, int >, pair< int, int > > return_move( pair< pair< int, int >, pair< int, int > > last_op_move );

};

#endif