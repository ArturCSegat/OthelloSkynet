#ifndef BOARD_H
#define BOARD_H

#include "coord.h"
#include "player.h"
#include <array>
#include <memory>
#include <vector>
#define GAME_N 8

class Board {
public:
    // char board[8][8];
    std::array<std::array<char, GAME_N>, GAME_N> board;
    char empty_square_marker;

    Board();
    Board(char esm);

    char& operator[] (Coord c);
    char const& operator[] (Coord c) const;
    void display() const;
    
    // fills fill with the coords to flip
    void playDiegonalLR(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playDiegonalRL(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playHorizontal(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;
    void playVertical(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const;

    // returns if this direction is valid
    bool willFlipInDiegonalLR(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2) const;
    bool willFlipInDiegonalRL(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2) const;
    bool willFlipInHorizontal(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2) const;
    bool willFlipInVertical(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2) const;
    

};

#endif
