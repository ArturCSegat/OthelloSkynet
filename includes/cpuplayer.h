#ifndef CPU_PLAYER_H
#define CPU_PLAYER_H

#include "board.h"
#include "game.h"
#include <queue>
#include <vector>


class BadCpuPlayer: public Player {
public:
    BadCpuPlayer(char p);
    ~BadCpuPlayer() override = default;
    
    Coord choseSquare(Game& game) override;
};

class CpuPlayer: public Player {
public:
    float aval_matrix[GAME_N][GAME_N];
    
    CpuPlayer(char p);
    ~CpuPlayer() override = default;
    
    Coord choseSquare(Game& game) override;
};

class BetterCpuPlayer : public CpuPlayer {
public:
    BetterCpuPlayer(char p);
    ~BetterCpuPlayer() override = default;
    
    Coord choseSquare(Game& game) override;
    int avaliateMoveTillEnd(Coord move, std::unique_ptr<Game> game);
};

class MaybeEvenBetterCpuPlayer : public BetterCpuPlayer {
public:
    MaybeEvenBetterCpuPlayer(char p);
    ~MaybeEvenBetterCpuPlayer() override = default;
    
    Coord choseSquare(Game& game) override;
private:
    int avaliateShallowTreeTillEnd(Coord move, std::unique_ptr<Game> game);
};

class MinMaxCpuPlayer : public BetterCpuPlayer {
public:
    MinMaxCpuPlayer(char p, float(*aval)(const Game& game, const MinMaxCpuPlayer *const self), int max_depth);
    ~MinMaxCpuPlayer() override = default;
    float(*aval)(const Game& game, const MinMaxCpuPlayer *const self); // custom heuristic
    int max_depth;

    Coord choseSquare(Game& game) override;
private:
    float Min(Game& game, Coord move, float alpha, float beta, int depth);
    float Max(Game& game, Coord move, float alpha, float beta, int depth);
};

#endif
