#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <algorithm>
#include <iostream>
#include <memory>


int main() {
    bool skip_1 = false;
    bool skip_2 = false;

    auto p1 = std::make_unique<CpuPlayer>(CpuPlayer('o'));
    auto p2 = std::make_unique<BetterCpuPlayer>(BetterCpuPlayer('x'));

    auto g = std::make_unique<Game>(Game(std::move(p2), std::move(p1), ' '));

    while(g->running) {
        g->board->display();
        g->printPlayerInfo();
        
        Coord move = g->players[g->curr_idx]->choseSquare(g);

        skip_1 = move == Coord{-1, -1};

        int fliped;

        fliped = g->play(move);

        if (fliped == -2) {
            g->board->display();
            g->endGame();
            break;
        }
        
        if (fliped == 0) {
            std::cout << "Must playa valid square, must flip at least 1 piecez\n";
            continue;
        }
        
        g->board->display();
        g->printPlayerInfo();

        Coord cpu_move = g->players[g->curr_idx]->choseSquare(g);

        skip_2 = cpu_move == Coord{-1, -1};

        if(skip_1 && skip_2) {
            std::cout << "Both skipped, no more moves availabe\n";
            g->endGame();
            break;
        }

        int over = g->play(cpu_move);

        if (over == -2 ) {
            g->board->display();
            g->endGame();
            break;
        }
    }
}
