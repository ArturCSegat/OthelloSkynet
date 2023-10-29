#ifndef CPU_PLAYER_H
#define CPU_PLAYER_H

#include "game.h"

class CpuPlayer: public Player {
public:
    float aval_rows[8];
    float aval_cols[8];

    CpuPlayer(char p);
    
    Coord choseSquare(const std::unique_ptr<Game>& game) override;
};

#endif
