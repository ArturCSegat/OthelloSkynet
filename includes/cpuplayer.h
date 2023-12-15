#ifndef CPU_PLAYER_H
#define CPU_PLAYER_H

#include "game.h"

class BadCpuPlayer: public Player {
public:
    BadCpuPlayer(char p);
    ~BadCpuPlayer() override = default;
    
    Coord choseSquare(Game& game) override;
};

class CpuPlayer: public Player {
public:
    float aval_matrix[8][8] = {
        { 10000.0f, 0.25f, 15.0f, 3.75f, 3.75f, 2.50f, 0.25f, 10000.0f },
        { 0.25f, 0.25f, 0.50f, 0.75f, 0.75f, 0.50f, 0.25f, 0.25f },
        { 15.0f, 0.50f, 5.00f, 5.00f, 5.00f, 5.00f, 0.50f, 15.0f },
        { 15.0f, 0.75f, 5.00f, 15.0f, 15.0f, 5.00f, 0.75f, 15.0f },
        { 15.0f, 0.75f, 5.00f, 15.0f, 15.0f, 5.00f, 0.75f, 15.0f },
        { 15.0f, 0.50f, 5.00f, 5.50f, 5.50f, 5.00f, 0.50f, 15.0f },
        { 0.25f, 0.25f, 0.50f, 0.75f, 0.75f, 0.50f, 0.25f, 0.25f },
        { 10000.0f, 0.25f, 15.0f, 15.0f, 15.0f, 15.0f, 0.25f, 10000.0f },
    };

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
    MinMaxCpuPlayer(char p, float(*aval)(const Game& game, float aval_matrix[8][8]));
    ~MinMaxCpuPlayer() override = default;
    float(*aval)(const Game& game, float aval_matrix[8][8]);

    Coord choseSquare(Game& game) override;
private:
    float Min(Game& game, Coord move, float alpha, float beta, int depth);
    float Max(Game& game, Coord move, float alpha, float beta, int depth);
};

#endif
