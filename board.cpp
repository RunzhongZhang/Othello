#include "board.h"
#include <iostream>
using namespace std;

/**
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/**
 * Destructor for the board.
 */
Board::~Board() {
}

/**
 * We referenced the tabe from 
 * https://github.com/kartikkukreja/blog-codes/blob/master/src/Heuristic%20Function%20for%20Reversi%20%28Othello%29.cpp
 */ 
const int Board::heuristic_values[64] =
    {20,-3,11, 8, 8,11,-3,20,
	 -3,-7,-4, 1, 1,-4,-7,-3,
	 11,-4, 2, 2, 2, 2,-4,11,
	  8, 1, 2,-3,-3, 2, 1, 8,
	  8, 1, 2,-3,-3, 2, 1, 8,
	 11,-4, 2, 2, 2, 2,-4,11,
	 -3,-7,-4, 1, 1,-4,-7,-3,
	 20,-3,11, 8, 8,11,-3,20};


/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

/**
Returns whether (x, y) is occupied.
*/
bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

/**
Returns whether (x, y) is occupied by the specified side.
*/
bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

/**
Sets position (x, y) to be occupied by the specified side.
*/
void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

/**
Flip a single piece at the specified position. Assumes the position is
occupied; i.e. there is something to flip there.
*/
void Board::flip(int x, int y) {
    black.flip(x + 8*y);
}

/**
Returns whether (x, y) is on the board.
*/
bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}



/**
Updates the board to reflect the specified move. Assumes the move is valid.
If the move is NULL, do not update the board.
*/
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/**
If the move is legal, returns a new Board where the move has been made.
The returned Board is dynamically allocated; please free with delete.
If the move is NOT legal, returns NULL.
Assumes the move is non-NULL.
*/
Board *Board::doMoveIfLegal(Move *m, Side side) {
    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return NULL;

    Board *newBoard = NULL;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) {
                    // This is a legal move

                    if (newBoard == NULL) {
                        // Initialize newBoard if it hasn't been yet.
                        newBoard = this->copy();
                        // err << "In doMoveIfLegal: Legal move (x, y) (" << X << ", " << Y << ")" << endl;
                    }

                    /*
                    Flip all pieces between (x, y), and (m.x, m.y).
                    */
                    for (int i = X + dx, j = Y + dy;
                         i != x || j != y;
                         i += dx, j += dy) {
                        newBoard->flip(i, j);
                        // cerr << "Flip (i, j) " << i << " " << j << endl;
                    }
                }
            }
        }
    }

    if (newBoard != NULL) {
        // Add new piece at (m.x, m.y)
        newBoard->set(side, X, Y);

        // cerr << "Resulting board:" << endl;
        // newBoard->printboard();
    } else {
        // cerr << "In doMoveIfLegal: Illegal move (x, y) (" << X << ", " << Y << ")" << endl;
    }

    return newBoard;
}

/**
Calculates the score, from the perspective of the specified side.
(Squares occupied by the same side as "side" contribute positively;
those occupied by the opposite side contribute negatively.)
*/
int Board::score(Side side) {
    int output = 0;
    
    output += 10 * piece_diff(side) + 801 * corner_occ(side) + 382 * corner_close(side) + 79 * mobility(side) + 74 * frontier(side) + 10 * heuristic_value(side);

    
    return output;
}


/**
 * Simply finds the stone differential, used in the endgame
 * board position score = (# stones you have) - (# stones your opponent has)
 */ 
int Board::score_endgame(Side side) {
	int output = 0;
    for (int i = 0; i < 64; i++) {
        if (taken[i]) {
            if (black[i] == (side == BLACK)) {
                /*
                if it's occupied by black and you are BLACK or
                if it's occupied by white and you are WHITE
                */
                output += 1;
            } else {
                output -= 1;
            }
        }
    }
    return output;
}

/**
 * Count total number of stones in the board
 */
int Board::countAll() {
    return taken.count();
}

/**
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/**
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/**
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/**
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

/**
 * Print the whole board
 * Blank-0
 * BLACK-B
 * WHITE-W
 */
void Board::printboard()
{
	for (int j = 0; j < 8; j++) {
        for (int i = 0; i < 8; i++) {
            if (!this->occupied(i, j))
            {
				cerr << "0 ";
			}
			else if (this->get(WHITE, i, j))
			{
				cerr << "W ";
			}
			else if (this->get(BLACK, i, j))
			{
				cerr << "B ";
			}
        }
        cerr << endl;
    }
    cerr << endl;
}

/**
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
	// Passing is only legal if you have no moves.
	if (m == NULL) return !hasMoves(side);
	int X = m->getX();
	int Y = m->getY();
	// Make sure the square hasn't already been taken.
	if (occupied(X, Y)) return false;
	Side other = (side == BLACK) ? WHITE : BLACK;
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dy == 0 && dx == 0) continue;
			// Is there a capture in that direction?
			int x = X + dx;
			int y = Y + dy;
			if (onBoard(x, y) && get(other, x, y)) {
				do {
					x += dx;
					y += dy;
				} while (onBoard(x, y) && get(other, x, y));
				if (onBoard(x, y) && get(side, x, y)) return true;
			}
		}
	}
	return false;
}

/**
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
	return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/**
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Move move(i, j);
			if (checkMove(&move, side)) return true;
		}
	}
	return false;
}

/**
 * return the number of valid moves
 */
int Board::valid_move(Side side)
{
	int count = 0;
	Move * new_move;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			new_move = new Move(i, j);
			if (checkMove(new_move, side))
			{
				count += 1;
			}
			
		}
	}
	return count;
}

/**
 * return the heuristic variable: mobility
 */ 
double Board::mobility(Side side)
{
	double mobility;
	Side other = (side == BLACK) ? WHITE : BLACK;
	int my_stone = valid_move(side);
	int opp_stone = valid_move(other);
	if (my_stone > opp_stone)
	{
		mobility = (100.0 * my_stone)/(my_stone + opp_stone);
	}
	else if (opp_stone > my_stone)
	{
		mobility = -(100.0 * opp_stone)/(my_stone + opp_stone);
	}
	else
	{
		mobility = 0;
	}
	return mobility;
}

/**
 * return the heuristic variable: piece_diff
 */
int Board::piece_diff(Side side)
{
	int piece_diff;
	if (side == BLACK)
	{
		if (countBlack()>countWhite())
		{
			piece_diff = (100.0 * countBlack())/(countBlack()+countWhite());
		}
		else if (countBlack()<countWhite());
		{
			piece_diff = -(100.0*countWhite())/(countBlack()+countWhite());
		}
	}
	else
	{
		if (countBlack()<countWhite())
		{
			piece_diff = (100.0 * countWhite())/(countBlack()+countWhite());
		}
		else if (countBlack()>countWhite());
		{
			piece_diff = -(100.0*countBlack())/(countBlack()+countWhite());
		}
	}	
	return piece_diff;
}

/**
 * return the heuristic variable: frontier
 */ 
int Board::frontier(Side side)
{
	int my_frontier = 0;
	int opp_frontier = 0;
	Side other = (side == BLACK) ? WHITE : BLACK;
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (!occupied(i, j))
			{
				if (onBoard(i-1,j-1))
				{
					if (get(side,i-1,j-1)) my_frontier++;
					else if (get(other,i-1,j-1)) opp_frontier++;
				}
				if (onBoard(i-1,j))
				{
					if (get(side,i-1,j)) my_frontier++;
					else if (get(other,i-1,j)) opp_frontier++;
				}
				if (onBoard(i-1,j+1))
				{
					if (get(side,i-1,j+1)) my_frontier++;
					else if (get(other,i-1,j+1)) opp_frontier++;
				}
				if (onBoard(i,j-1))
				{
					if (get(side,i,j-1)) my_frontier++;
					else if (get(other,i,j-1)) opp_frontier++;
				}
				if (onBoard(i,j+1))
				{
					if (get(side,i,j+1)) my_frontier++;
					else if (get(other,i,j+1)) opp_frontier++;
				}
				if (onBoard(i+1,j-1))
				{
					if (get(side,i+1,j-1)) my_frontier++;
					else if (get(other,i+1,j-1)) opp_frontier++;
				}
				if (onBoard(i+1,j))
				{
					if (get(side,i+1,j)) my_frontier++;
					else if (get(other,i+1,j)) opp_frontier++;
				}
				if (onBoard(i+1,j+1))
				{
					if (get(side,i+1,j+1)) my_frontier++;
					else if (get(other,i+1,j+1)) opp_frontier++;
				}
			}
		}
	}
	if (my_frontier > opp_frontier)
		return (100.0*my_frontier)/(my_frontier + opp_frontier);
	else if (my_frontier < opp_frontier)
		return -(100.0*opp_frontier)/(my_frontier + opp_frontier);
	else
		return 0;
	
}

/**
 * return the heuristic variable: corner_occupancy
 */
int Board::corner_occ(Side side)
{
	int black_occ = 0;
	int white_occ = 0;
	
	if (get(BLACK,0,0))
		black_occ++;
    else if(get(WHITE,0,0))
		white_occ++;
		
	if (get(BLACK,0,7))
		black_occ++;
    else if(get(WHITE,0,7))
		white_occ++;
		
	if (get(BLACK,7,0))
		black_occ++;
    else if(get(WHITE,7,0))
		white_occ++;
		
	if (get(BLACK,7,7))
		black_occ++;
    else if(get(WHITE,7,0))
		white_occ++;
		
	if (side == BLACK)
		return 25*(black_occ - white_occ);
	else
		return 25*(white_occ - black_occ);
}

/**
 * return the heuristic variable: corner closeness (only if corner is not occupied)
 */ 
 
int Board::heuristic_value(Side side)
{
	int heuristic_value = 0;
    for (int i = 0; i < 64; i++) {
        if (taken[i]) {
            if (black[i] == (side == BLACK)) {
                /*
                if it's occupied by black and you are BLACK or
                if it's occupied by white and you are WHITE
                */
                heuristic_value += heuristic_values[i];
            } else {
                heuristic_value -= heuristic_values[i];
            }
        }
    }
    return heuristic_value;
}

int Board::corner_close(Side side)
{
	int my_close = 0;
	int opp_close = 0;
	Side other = (side == BLACK) ? WHITE : BLACK;
	if (!occupied(0,0))
	{
		if (get(side, 0, 1)) 
			my_close++;
		else if (get(other, 0,1))
			opp_close++;
		if (get(side, 1, 1)) 
			my_close++;
		else if (get(other, 1,1))
			opp_close++;
		if (get(side, 1, 0)) 
			my_close++;
		else if (get(other, 1,0))
			opp_close++;
	}
	
	if (!occupied(0,7))
	{
		if (get(side, 0, 6)) 
			my_close++;
		else if (get(other, 0,6))
			opp_close++;
		if (get(side, 1, 6)) 
			my_close++;
		else if (get(other, 1,6))
			opp_close++;
		if (get(side, 1, 7)) 
			my_close++;
		else if (get(other, 1,7))
			opp_close++;
	}
	
	if (!occupied(7,0))
	{
		if (get(side, 7, 1)) 
			my_close++;
		else if (get(other, 7,1))
			opp_close++;
		if (get(side, 6, 1)) 
			my_close++;
		else if (get(other, 6,1))
			opp_close++;
		if (get(side, 6, 0)) 
			my_close++;
		else if (get(other, 6,0))
			opp_close++;
	}
	
	if (!occupied(7,7))
	{
		if (get(side, 6, 7)) 
			my_close++;
		else if (get(other, 6,7))
			opp_close++;
		if (get(side, 6, 6)) 
			my_close++;
		else if (get(other, 6,6))
			opp_close++;
		if (get(side, 7, 6)) 
			my_close++;
		else if (get(other, 7,6))
			opp_close++;
	}
	return -12.5 * (my_close - opp_close);
}
