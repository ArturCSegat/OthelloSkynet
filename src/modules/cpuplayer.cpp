#include "../../includes/cpuplayer.h"
#include "../../includes/game.h"
#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>


CpuPlayer::CpuPlayer(char p): Player(p){
    // extremidades tem valor 2
    // casas que precedem uma extremidade tem valor 0.5 pois possibilitam ao adversário acessar a extremidade
    // centro tem valor aumentado

    aval_rows[0] = 2;
    aval_rows[1] = 0.5;
    aval_rows[2] = 1;
    aval_rows[3] = 1.5;
    aval_rows[4] = 1.5;
    aval_rows[5] = 1;
    aval_rows[6] = 0.5;
    aval_rows[7] = 2;

    aval_cols[0] = 2;
    aval_cols[1] = 0.5;
    aval_cols[2] = 1;
    aval_cols[3] = 1.5;
    aval_cols[4] = 1.5;
    aval_cols[5] = 1;
    aval_cols[6] = 0.5;
    aval_cols[7] = 2;
}

Coord CpuPlayer::choseSquare(const std::unique_ptr<Game>& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((*game->board)[Coord{i, j}] != game->board->empty_square_marker) {
                continue;
            }
            int flipped = game->clone()->play(Coord{i, j}); 
            if (flipped == 0) {
                continue;
            }
            
            int fit = (aval_rows[i] * aval_cols[j]) * ((float)flipped / piece_count);
    // fit =   (val linha   +  valor coluna) * (quantas peças a jogada flipa / quantas peças o jogador tem antes de jogar)

            
            moves[fit] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    return best_move;

}
