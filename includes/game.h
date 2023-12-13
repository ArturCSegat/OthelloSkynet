#ifndef GAME_H

#include "coord.h"
#include <vector>
#include "board.h"
#include "player.h"
#include <memory>
#include <stack>

#define GAME_H
class Game {
public:
    Board board;
    int age_matrix[8][8];
    std::unique_ptr<Player> players[2];
    std::stack<std::vector<Coord>> flips;
    int curr_idx;
    int play_count;
    bool running;

    Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square);

    std::vector<Coord> flipedFromMove(Coord move, int player_idx) const;

    int play(Coord c);
    void undo();

    std::unique_ptr<Game> clone() const;

    void endGame();

    void printPlayerInfo();

    // returns the avaliation of a givenn player
    float playerAval(float aval_matrix[8][8]);
};

#endif
