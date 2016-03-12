#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
using namespace std;

class Board {
   
private:
    bitset<64> black;
    bitset<64> taken;    
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    void flip(int x, int y);
    bool onBoard(int x, int y);
      
public:
    Board();
    ~Board();
    Board *copy();
    
    static const int heuristic_values[64];
    
    bool checkMove(Move *m, Side side);
    bool isDone();
    bool hasMoves(Side side);
    void doMove(Move *m, Side side);
    Board *doMoveIfLegal(Move *m, Side side);
    int score(Side side);
    int score_endgame(Side side);
    int countAll();
    int count(Side side);
    int countBlack();
    int countWhite();
    void setBoard(char data[]);
    void printboard();
    int valid_move(Side side);
    double mobility(Side side);
    int piece_diff(Side side);
    int corner_occ(Side side);
    int corner_close(Side side);
    double frontier(Side side);
    int heuristic_value(Side side);
};

#endif
