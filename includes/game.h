#include "coord.h"
#include <vector>
#ifndef GAME_H

#include "board.h"
#include "player.h"
#include <memory>

#define GAME_H
class Game {
public:
    std::unique_ptr<Board> board;
    std::unique_ptr<Player> players[2];
    int curr_idx;
    bool running;

    Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square);

    std::vector<Coord> flipedFromMove(Coord move);

    int play(Coord c);

    std::unique_ptr<Game> clone();

    void endGame();

    void printPlayerInfo();
};

#endif
