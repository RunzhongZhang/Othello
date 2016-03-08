#include "player.h"
#include <vector>
#include <stdlib.h>
#include <limits.h>

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

/**
Returns the maximal score of the board, and modifies best_move to contain 
the move that will reach that score. (best_move is dynamically allocated; 
please free with delete.) Uses minimax of the specified depth, where the
 to-be-optimized player is on the specified side. 

If depth = 0 or there are no legal moves, the board's heuristic score is 
returned, and best_move is NULL and does NOT need to be deleted.
*/
int Player::minimax(Board *board, Side side, int depth, Move *&best_move) {
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
                new_score = -minimax(new_board, otherSide, depth - 1, garbage);
                if (garbage != NULL) {
                    delete garbage;
                }
                /*
                cerr << endl << "---------------" << endl;
                cerr << "In minimax: LEGAL move: x y " << new_move->getX() << " " << new_move->getY() << endl;
                cerr << "Resulting score: " << new_min_score << endl;
                cerr << "Resulting board:" << endl;
                newBoard->printboard();
                */
                if (new_score > best_score) { 
                    // Best score so far; update best_score and best_move
                    // cerr << "In minimax: New best move: x y " << new_move->getX() << " " << new_move->getY() << endl;
                    // cerr << "Resulting score: " << new_score << endl;
                    // cerr << "Resulting board:" << endl;
                    // board->printboard();
                    //cerr << "Preceding move updated as BEST!" << endl;
                    
                    best_score = new_score;
                    if (best_move != NULL) {
                        delete best_move;
                    }
                    best_move = new_move;
                    
                } else {
                    // This move wasn't the best so far, delete it
                    delete new_move;
                }
                
            } else {
                // This move wasn't legal, delete it
                delete new_move;
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

// /**
// Returns the best move for the specified board using minimax of the
// specified depth, if the player is on the specified side.
// Assumes depth >= 1. Returns NULL if there are no legal moves.
// */
// void Player::minimax(Board *board, Side side, int depth, Move *&best_move) {    
//     int best_min_score = -1000000;
//     int new_min_score;
//     Move *new_move;
//     best_move = NULL;
//     Board *newBoard;
//     
//     Side otherSide = side == BLACK ? WHITE : BLACK;
//     
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             new_move = new Move(i, j);
//             newBoard = board->doMoveIfLegal(new_move, side);
//             
//             if (newBoard != NULL) {                
//                 // Find its heuristic minimum score, using recursive minimax
//                 new_min_score = -minimax_rec(newBoard, otherSide, depth - 1);
//                 if (new_min_score > best_min_score) { 
//                     // Best move so far; update best_move                    
//                     best_min_score = new_min_score;
//                     if (best_move != NULL) {
//                         delete best_move;
//                     }
//                     best_move = new_move;
//                     
//                 } else {
//                     // This move wasn't the best so far, delete it
//                     delete new_move;
//                 }
//                 
//             } else {
//                 // This move wasn't legal, delete it
//                 delete new_move;
//             }
//             
//             delete newBoard;
//         }
//     }
// }


// /**
// Returns the best move for the specified board using minimax of the
// specified depth, if the player is on the specified side.
// Assumes depth >= 1. Returns NULL if there are no legal moves.
// */
// Move *Player::minimax(Board *board, Side side, int depth) {    
//     int best_min_score = -1000000;
//     int new_min_score;
//     Move *new_move, *best_move = NULL;
//     Board *newBoard;
//     
//     Side otherSide = side == BLACK ? WHITE : BLACK;
//     
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             new_move = new Move(i, j);
//             newBoard = board->doMoveIfLegal(new_move, side);
//             
//             if (newBoard != NULL) {                
//                 // Find its heuristic minimum score, using recursive minimax
//                 new_min_score = -minimax_rec(newBoard, otherSide, depth - 1);
//                 /*
//                 cerr << endl << "---------------" << endl;
//                 cerr << "In minimax: LEGAL move: x y " << new_move->getX() << " " << new_move->getY() << endl;
//                 cerr << "Resulting score: " << new_min_score << endl;
//                 cerr << "Resulting board:" << endl;
//                 newBoard->printboard();
//                 */
//                 if (new_min_score > best_min_score) { 
//                     // Best move so far; update best_move
//                     /*
//                     cerr << "In minimax: New best move: x y " << new_move->getX() << " " << new_move->getY() << endl;
//                     cerr << "Resulting score: " << new_min_score << endl;
//                     cerr << "Resulting board:" << endl;
//                     board->printboard();
//                     */
//                     //cerr << "Preceding move updated as BEST!" << endl;
//                     
//                     best_min_score = new_min_score;
//                     if (best_move != NULL) {
//                         delete best_move;
//                     }
//                     best_move = new_move;
//                     
//                 } else {
//                     // This move wasn't the best so far, delete it
//                     delete new_move;
//                 }
//                 
//             } else {
//                 // This move wasn't legal, delete it
//                 delete new_move;
//             }
//             
//             delete newBoard;
//         }
//     }
//     
//     return best_move;
// }
// 
// 
// /**
// Returns the score of the specified board. 
// Use recursive minimax to calculate a heuristic score of the specified board.
// */
// int Player::minimax_rec(Board *board, Side side, int depth) {
//     
//     if (depth == 0) {
//         // Base case
//         int score = board->score(side); //??? Yourside or just side???
//         return score;
//     }
//     
//     int best_min_score = -1000000;
//     int new_min_score;
//     Move *new_move;
//     bool exists_legal_move = false; 
// 
//     Board *new_board;
//     
//     Side otherSide = side == BLACK ? WHITE : BLACK;
//     
//     for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             new_move = new Move(i, j);
//             new_board = board->doMoveIfLegal(new_move, side);
//             if (new_board != NULL) {
//                 // move is a legal move.
//                 exists_legal_move = true;
//                 
//                 // Find its heuristic minimum score, using recursive minimax
//                 new_min_score = -minimax_rec(new_board, otherSide, depth - 1);
//                 if (new_min_score > best_min_score) {
//                     // Best move so far; update best_min_score
//                     best_min_score = new_min_score;
//                 }
//             }
//             delete new_move;
//             delete new_board;
//         }
//     }
//     
//     if (exists_legal_move) { 
//         //There was a legal move
//         return best_min_score;
//     }
//     
//     //Else: this node is a terminal node
//     return board->score(player_side);
// } 


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
    // cerr << "Start doMove =====================" << endl;
    // 
    // cerr << "Starting board: " << endl;
    // board->printboard();
    
    Side opponent_side = (player_side == BLACK) ? WHITE : BLACK;
	
	// Update the board with opponent's move
	board->doMove(opponentsMove, opponent_side);
    
    // cerr << "Updated board: " << endl;
    // board->printboard();

    // Find best move, and update our board with it
    Move *best_move;
    minimax(board, player_side, 5, best_move);
    board->doMove(best_move, player_side);
    
    // if (best_move == NULL) {
    //     cerr << "In Player::doMove: PASS" << endl;
    // } else {
    //     cerr << "In Player::doMove: Best move found: x y " << best_move->getX() << " " << best_move->getY() << endl;
    // }
    // cerr << "Resulting score: " << score << endl;
    // cerr << "Resulting board:" << endl;
    // board->printboard();
    
    return best_move;
    
    
    
//     // Find the opponent's side
//     Side opponent_side;
//     if (player_side == BLACK) {
// 		opponent_side = WHITE;
// 	}
// 	else {
// 		opponent_side = BLACK;
// 	}
// 	
// 	// Update the board
// 	board->doMove(opponentsMove, opponent_side);
// 	board->printboard();
// 	//cerr << "here1";
// 	
// 	// If there is not valid move, return NULL
//     if (board->hasMoves(player_side) == false){
// 		return NULL;
// 	}
// 	
// 	// Create a valid_move vector to store all the valid moves of the player
// 	vector<Move*> valid_move;
// 
// 	// Find all the valid moves and store them in the vector valid_move
// 	for (int i = 0; i < 8; i++) {
//         for (int j = 0; j < 8; j++) {
//             Move * move = new Move(i, j);
//             if (board->checkMove(move, player_side)) 
//             {
// 				valid_move.push_back(move);
// 			}
// 			else
// 			{
// 				delete move;
// 			}
//         }
//     }
//     
//     // Get the minimax move with 2 level depth
// 
//     int score_min = 1 << 30;
//     int score_max = 1 << 31;
// 	int score;
// 	Move * best_move = NULL;
//     Board *board2 = board->copy();
//     for (unsigned int i = 0; i < valid_move.size(); i++) {
//         board2->doMove(valid_move[i], player_side);
//         score_min = 1 << 30;
//         for (int j = 0; j < 8; j++) {
// 			for (int k = 0; k < 8; k++) {
// 				Move * move2 = new Move(j, k);
// 				if (board2->checkMove(move2, opponent_side)) 
// 				{
// 					Board *board3 = board2->copy();
// 					board3->doMove(move2, opponent_side);
// 					score = board3->score(player_side);
// 					if (score < score_min)
// 					{
// 						score_min = score;
// 					}
// 				}
// 				else
// 				{
// 					delete move2;
// 				}
// 			}
// 		}
// 		if (score_min > score_max)
// 		{
// 			score_max = score_min;
// 			best_move = valid_move[i];
// 		}
//     }
//     
//     board->doMove(best_move, player_side); //update the board
// 	board->printboard();
// 	//cerr << "here2";
// 	return best_move;
}
