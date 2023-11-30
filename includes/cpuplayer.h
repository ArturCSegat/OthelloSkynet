#ifndef CPU_PLAYER_H
#define CPU_PLAYER_H

#include "game.h"

class BadCpuPlayer: public Player {
public:
    BadCpuPlayer(char p);
    ~BadCpuPlayer() override = default;
    
    Coord choseSquare(const Game& game) override;
};

class CpuPlayer: public Player {
public:
    float aval_rows[8];
    float aval_cols[8];

    CpuPlayer(char p);
    ~CpuPlayer() override = default;
    
    Coord choseSquare(const Game& game) override;
};

class BetterCpuPlayer : public CpuPlayer {
public:
    BetterCpuPlayer(char p);
    ~BetterCpuPlayer() override = default;
    
    Coord choseSquare(const Game& game) override;
    int avaliateMoveTillEnd(Coord move, std::unique_ptr<Game> game);
};

class MaybeEvenBetterCpuPlayer : public BetterCpuPlayer {
public:
    MaybeEvenBetterCpuPlayer(char p);
    ~MaybeEvenBetterCpuPlayer() override = default;
    
    Coord choseSquare(const Game& game) override;
private:
    int avaliateShallowTreeTillEnd(Coord move, std::unique_ptr<Game> game);
};

class MinMaxCpuPlayer : public BetterCpuPlayer {
public:
    MinMaxCpuPlayer(char p);
    ~MinMaxCpuPlayer() override = default;

    Coord choseSquare(const Game& game) override;
private:
    float Min(Game& game, Coord move, int depth);
    float Max(Game& game, Coord move, int depth);
};

#endif
