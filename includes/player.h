#ifndef PLAYER_H
#define PLAYER_H

#include "coord.h"
#include <memory>

// just here so the linker wont die
class Game;

class Player {
public:
    char piece;
    int piece_count;
    int play_count;

    Player(char p);
    virtual ~Player() = default;

    virtual Coord choseSquare(Game& game);
};

#endif
