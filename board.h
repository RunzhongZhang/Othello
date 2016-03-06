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
    
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    Board *doMoveIfLegal(Move *m, Side side);
    int score(Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    int getscore(Move *m, Side side);
    void setBoard(char data[]);
    void printboard();
};

#endif
