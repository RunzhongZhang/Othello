#ifndef __PLAYER_H__ 
#define __PLAYER_H__

#include <iostream>
#include <stdlib.h>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    /*
     * Depth to search before the endgame.
     */
    static const int DEPTH = 8;
    /*
     * If there are only this many empty spaces on the board,
     * use complete endgame solver.
     */
    static const int DEPTH_ENDGAME = 16;
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int minimax(Board *board, Side side, int depth, int lower_bound,
            int upper_bound, Move *&best_move);
    int minimax_endgame(Board *board, Side side, int lower_bound,
            int upper_bound, Move *&best_move);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side player_side;
    Board * board;
};

#endif
