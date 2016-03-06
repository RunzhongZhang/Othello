#include "board.h"
#include <iostream>
using namespace std;

/*
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

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/**
Heuristic values for each square in the board.
*/
const int Board::heuristic_values[64] = 	
    {50000,-1500,300,100,100,300,-150, 50000,
	 -1500,-2500, 0, 0, 0, 0,-2500,-1500,
	   300,   0, 50, 20, 20, 50,   0,  300,
	   100,   0, 20,16,16, 20,   0,  100,
	   100,   0, 20,16,16, 20,   0,  100,
	   300,   0, 50, 20, 20, 50,   0,  300,
	 -1500,-2500, 0, 0, 0, 0,-2500,-1500,
	  50000,-1500,300,100,100,300,-1500, 50000};

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

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


bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
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

/*
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

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

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
Calculates the score, if You are on the specified side. 
(Squares occupied by the same side as "side" contribute positively; 
those occupied by the opposite side contribute negatively.)
*/
int Board::score(Side yourSide) {
    int output;
    for (int i = 0; i < 64; i++) {
        if (taken[i]) {
            if (black[i] == (yourSide == BLACK)) {
                /*
                if it's occupied by black and you are BLACK or
                if it's occupied by white and you are WHITE
                */
                output += heuristic_values[i];
            } else {
                output -= heuristic_values[i];
            }
        }
    }
    return output;
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
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

/*
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

