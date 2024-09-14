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
    int age_matrix[GAME_N][GAME_N];
    std::unique_ptr<Player> players[2];
    std::stack<std::vector<Coord>> flips;
    int curr_idx;
    int play_count;
    bool running;
    std::vector<Coord> to_flip;

    Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square);

    bool isValid(Coord move, int player_idx) const;
    std::vector<Coord> available() const;

    // returns how many needs to be fliped and sets this->to_flip to the respective coords
    int flipedFromMove(Coord move, int player_idx);

    // plays the given Coord, if calc is true, will calculate this->to_flip, if you've just ran flipeFromMove for Coord c, set it to false
    int play(Coord c);
    void undo();

    std::unique_ptr<Game> clone() const;
    void clone_into(Game& into) const;

    void endGame();

    void printPlayerInfo() const;
    bool isOver() const;

};

#endif
