#include "player.h"
#include <vector>

/*
// Create a static heuristic value table
static int heuristic_value[64] = 
	{ 500,-150,30,10,10,30,-150, 500,
	 -150,-250, 0, 0, 0, 0,-250,-150,
	   30,   0, 1, 2, 2, 1,   0,  30,
	   10,   0, 2,16,16, 2,   0,  10,
	   10,   0, 2,16,16, 2,   0,  10,
	   30,   0, 1, 2, 2, 1,   0,  30,
	 -150,-250, 0, 0, 0, 0,-250,-150,
	  500,-150,30,10,10,30,-150, 500};
*/

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
	//board->printboard();
	//cerr << "here1";
	
	// If there is not valid move, return NULL
    if (board->hasMoves(player_side) == false){
		return NULL;
	}
	
	// Create a valid_move vector to store all the valid moves of the player
	vector<Move*> valid_move;

	// Find all the valid moves and store them in the vector valid_move
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move * move = new Move(i, j);
            if (board->checkMove(move, player_side)) 
            {
				valid_move.push_back(move);
			}
			else
			{
				delete move;
			}
        }
    }
    
    // Get the minimax move with 2 level depth
    int score_max = -1000;
    int score_min = 1000;
	int score;
	Move * best_move = NULL;
    Board *board2 = board->copy();
    for (unsigned int i = 0; i < valid_move.size(); i++) {
        board2->doMove(valid_move[i], player_side);
        score_min = 1000;
        for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				Move * move2 = new Move(j, k);
				if (board2->checkMove(move2, opponent_side)) 
				{
					Board *board3 = board2->copy();
					board3->doMove(move2, opponent_side);
					score = -board3->getscore(move2, opponent_side);
					if (score < score_min)
					{
						score_min = score;
					}
				}
				else
				{
					delete move2;
				}
			}
		}
		if (score_min > score_max)
		{
			score_max = score_min;
			best_move = valid_move[i];
		}
    }
    
    board->doMove(best_move, player_side); //update the board
	//board->printboard();
	//cerr << "here2";
	return best_move;
}
