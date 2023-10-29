#ifndef PLAYER_H
#define PLAYER_H

#include "coord.h"
#include <memory>

class Game;

class Player {
public:
    char piece;
    int piece_count;

    Player(char p);

    virtual Coord choseSquare(const std::unique_ptr<Game>& game);
};

#endif
