#ifndef GAME_H

#include "coord.h"
#include <vector>
#include "board.h"
#include "player.h"
#include <memory>

#define GAME_H
class Game {
public:
    Board board;
    std::unique_ptr<Player> players[2];
    int curr_idx;
    bool running;

    Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square);

    std::vector<Coord> flipedFromMove(Coord move, int player_idx) const;

    int play(Coord c);

    std::unique_ptr<Game> clone() const;

    void endGame();

    void printPlayerInfo();

    // returns the avaliation of a givenn player
    int playerAval(float * aval_rows, float * aval_cols);
};

#endif
