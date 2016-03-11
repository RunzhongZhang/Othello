#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
	player_side = side;
	board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
} 

/**
Returns the maximal score of the board, and modifies best_move to contain 
the move that will reach that score. (best_move is dynamically allocated; 
please free with delete.) Uses minimax of the specified depth, where the
to-be-optimized player is on the specified side. 

If depth = 0 or there are no legal moves, the board's heuristic score is 
returned, and best_move is NULL and does NOT need to be deleted.
*/
int Player::minimax(Board *board, Side side, int depth, int lower_bound, 
        int upper_bound, Move *&best_move) {
    best_move = NULL;
    
    if (depth == 0) {
        // Base case: return score from the perspective of "side"
        return board->score(side);
    }

    int best_score = -1000000;
    int new_score;
    Move *new_move, *garbage;
    Board *new_board;
    
    Side otherSide = side == BLACK ? WHITE : BLACK;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            new_move = new Move(i, j);
            new_board = board->doMoveIfLegal(new_move, side);
            
            if (new_board != NULL) {
                // Legal move
                             
                // Find heuristic score of new_board, using recursive minimax
                new_score = -minimax(new_board, otherSide, depth - 1, 
                        -upper_bound, -lower_bound, garbage);
                if (garbage != NULL) {
                    delete garbage;
                }
                
                // cerr << endl << "---------------" << endl;
                // cerr << "In minimax: LEGAL move: x y " << new_move->getX() << " " << new_move->getY() << endl;
                // cerr << "Resulting score: " << new_min_score << endl;
                // cerr << "Resulting board:" << endl;
                // newBoard->printboard();
                
                if (new_score > best_score) { 
                    // Best score so far; update best_score and best_move
                    // cerr << "Preceding move updated as BEST!" << endl;
                    
                    best_score = new_score;
                    if (best_move != NULL) {
                        delete best_move;
                    }
                    best_move = new_move;
                    
                } else {
                    // This move wasn't the best so far, delete it
                    delete new_move;
                }

                // lower_bound = max(lower_bound, new_score)
                if (new_score > lower_bound) {
                    lower_bound = new_score;
                }

                
            } else {
                // This move wasn't legal, delete it
                delete new_move;
            }

            if (lower_bound >= upper_bound) { 
                return best_score;
            }
            
            delete new_board;
        }
    }
    
    if (best_move == NULL) { 
        //There were no legal moves
        return board->score(side);
    }
    
    return best_score;
}

/**
Returns the maximal score of the board, and modifies best_move to contain 
the move that will reach that score. (best_move is dynamically allocated; 
please free with delete.) Uses minimax of the specified depth, where the
to-be-optimized player is on the specified side. 

If depth = 0 or there are no legal moves, the board's heuristic score is 
returned, and best_move is NULL and does NOT need to be deleted.
*/
int Player::minimax_endgame(Board *board, Side side, int lower_bound, 
        int upper_bound, Move *&best_move) {
    best_move = NULL;
    int best_score = -1000000;
    int new_score;
    Move *new_move, *garbage;
    Board *new_board;

    Side otherSide = side == BLACK ? WHITE : BLACK;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            new_move = new Move(i, j);
            new_board = board->doMoveIfLegal(new_move, side);

            if (new_board != NULL) {
                // Legal move

                // Find heuristic score of new_board, using recursive minimax
                new_score = -minimax_endgame(new_board, otherSide, -upper_bound, 
                        -lower_bound, garbage);
                if (garbage != NULL) {
                    delete garbage;
                }

                if (new_score > best_score) { 
                    // Best score so far; update best_score and best_move
                    // cerr << "Preceding move updated as BEST!" << endl;

                    best_score = new_score;
                    if (best_move != NULL) {
                        delete best_move;
                    }
                    best_move = new_move;

                } else {
                    // This move wasn't the best so far, delete it
                    delete new_move;
                }

                // lower_bound = max(lower_bound, new_score)
                if (new_score > lower_bound) {
                    lower_bound = new_score;
                }


            } else {
                // This move wasn't legal, delete it
                delete new_move;
            }

            if (lower_bound >= upper_bound) { 
                return best_score;
            }

            delete new_board;
        }
    }

    if (best_move == NULL) { 
        //There were no legal moves. Maybe the board was full
        return board->score_endgame(side);
    }

    return best_score;
}



/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {        
    Side opponent_side = (player_side == BLACK) ? WHITE : BLACK;

	// Update the board with opponent's move
	board->doMove(opponentsMove, opponent_side);
    
    // Find best move, and update our board with it
    Move *best_move;
    if (64 - board->countAll() <= DEPTH_ENDGAME) {
        // Use endgame solver
        int num_pieces = board->countAll();
        cerr << num_pieces << " pieces on board; use endgame solver" << endl;
        minimax_endgame(board, player_side, -1000000, +1000000, best_move);
    } else {
        minimax(board, player_side, DEPTH, -1000000, +1000000, best_move); 
    }
    board->doMove(best_move, player_side);
    
    return best_move;
}
