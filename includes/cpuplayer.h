#ifndef CPU_PLAYER_H
#define CPU_PLAYER_H

#include "game.h"

class BadCpuPlayer: public Player {
public:
    BadCpuPlayer(char p);
    
    Coord choseSquare(const std::unique_ptr<Game>& game) override;
};

class CpuPlayer: public Player {
public:
    float aval_rows[8];
    float aval_cols[8];

    CpuPlayer(char p);
    
    Coord choseSquare(const std::unique_ptr<Game>& game) override;
};

class BetterCpuPlayer : public CpuPlayer {
public:
    BetterCpuPlayer(char p);
    
    Coord choseSquare(const std::unique_ptr<Game>& game) override;
private:
    int avaliateMoveTillEnd(Coord move, const std::unique_ptr<Game>& game);
};

#endif
