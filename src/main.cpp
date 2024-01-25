#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include "../includes/avals.cpp"
#include <math.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

int main() {
    srand (static_cast <unsigned> (time(0)));
    auto p1 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('o', aval2, 5));
    auto p2 = std::make_unique<MctsCpuPlayer>(MctsCpuPlayer('x', rollout2, rollout2, 5000));

    auto g = Game(std::move(p2), std::move(p1), ' ');

    while(g.running) {
        g.board.display();
        g.printPlayerInfo();

        Coord move = g.players[g.curr_idx]->choseSquare(g);

        if (move == Coord{-1, -1}) {
            std::cout << "Skipped\n";
        }
        if (move == Coord{-3, -3}) {
            g.undo();
            continue;
        }

        int fliped;

        fliped = g.play(move);

        if (fliped == -2) {
            g.board.display();
            g.endGame();
            break;
        }

        if (fliped == 0) {
            std::cout << "Bad move: " << move.toString() << " Must playa valid square, must flip at least 1 piecez\n";
            continue;
        }

        g.board.display();
        g.printPlayerInfo();

        Coord cpu_move = g.players[g.curr_idx]->choseSquare(g);

        if (cpu_move == Coord{-3, -3}) {
            g.undo();
            continue;
        }
        if (cpu_move == Coord{-1, -1}) {
            std::cout << "Skipped\n";
        }

        if(cpu_move == move && move == Coord{-1, -1}) {
            std::cout << "Both skipped, no more moves availabe\n";
            g.endGame();
            break;
        }

        int over = g.play(cpu_move);

        if (over == -2 ) {
            g.board.display();
            g.endGame();
            break;
        }
    }
}
