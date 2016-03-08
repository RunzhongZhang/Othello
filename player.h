#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    int minimax(Board *board, Side side, int depth, Move *&best_move);
    Move *minimax(Board *board, Side side, int depth);
    int minimax_rec(Board *board, Side side, int depth);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side player_side;
    Board * board;
};

#endif
