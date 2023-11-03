#ifndef BOARD_H
#define BOARD_H

#include "coord.h"
#include "player.h"
#include <memory>
#include <vector>

class Board {
public:
    char board[8][8];
    char empty_square_marker;
    Board(char esm);

    char& operator[] (Coord c);
    void display();
    
    void playDiegonalLR(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill);
    void playDiegonalRL(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill);
    void playVertical(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill);
    void playHorizontal(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill);
};

#endif
