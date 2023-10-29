#include "../../includes/coord.h"
#include "../../includes/player.h"
#include <iostream>

Player::Player(char p) {
    piece = p;
    piece_count = 0;
}

Coord Player::choseSquare() {
    std::cout << "enter thee coords, player: " << piece << "\n";
    int i, j;
    scanf("%d %d", &i, &j);

    return Coord{i, j};
}

