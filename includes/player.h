#ifndef PLAYER_H
#define PLAYER_H

#include "coord.h"
#include <memory>

class Player {
public:
    char piece;
    int piece_count;

    Player(char p);

    virtual Coord choseSquare();
};

#endif
