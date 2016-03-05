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
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
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
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    
    // Find the opponent's side
    Side opponent_side;
    if (player_side == BLACK) {
		opponent_side = WHITE;
	}
	else {
		opponent_side = BLACK;
	}
	
	// Update the board
	board->doMove(opponentsMove, opponent_side);
	
	// If there is not valid move, return NULL
    if (board->hasMoves(player_side) == false){
		return NULL;
	}
	
	// Return the move with the highest score
	int score_max = -65;
	int score;
	Move * best_move = NULL;
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move * move = new Move(i, j);
            if (board->checkMove(move, player_side)) 
            {
				score = board->getscore(move, player_side);
				if (score > score_max)
				{
					if (best_move != NULL)	delete best_move;
					best_move = move;
					score_max = score;
				}
				else
				{
					delete move;
				}
			}
        }
    }
    board->doMove(best_move, player_side);
	return best_move;
    
	return NULL;
	
    
}
