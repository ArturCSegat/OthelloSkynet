#include "../../includes/game.h"
#include "../../includes/cpuplayer.h"
#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>


CpuPlayer::CpuPlayer(char p): Player(p){
    for (int i = 0; i < 8; i++) {
        aval_rows[i] = (float)i;
        aval_cols[i] = (float)i;
    }
}

Coord CpuPlayer::choseSquare(const std::unique_ptr<Game>& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*game->board)[Coord{i, j}] != game->board->empty_square_marker) {
                continue;
            }
            int r = game->clone()->play(Coord{i, j}); 
            if (r == 0) {
                continue;
            }
            moves[r] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return best_move;

}
