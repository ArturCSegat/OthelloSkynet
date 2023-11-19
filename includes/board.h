#ifndef BOARD_H
#define BOARD_H

#include "coord.h"
#include "player.h"
#include <array>
#include <memory>
#include <vector>

class Board {
public:
    // char board[8][8];
    std::array<std::array<char, 8>, 8> board;
    char empty_square_marker;

    Board();
    Board(char esm);

    char& operator[] (Coord c);
    char const& operator[] (Coord c) const;
    void display();
    
    void playDiegonalLR(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playDiegonalRL(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playHorizontal(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playVertical(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
};

#endif
