#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include "../includes/treenode.h"
#include "../includes/avals.cpp"
#include <ctime>
#include <math.h>
#include <iostream>
#include <memory>

int main() {
    srand (static_cast <unsigned> (time(0)));
    auto p1 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('o', aval, 5));
    auto p2 = std::make_unique<MctsCpuPlayer3>(MctsCpuPlayer3('x', rollout, 2000, 0));

    auto g = Game(std::move(p1), std::move(p2), ' ');

    auto p1_time = 0;
    auto p2_time = 0;
    auto p1_count = 0;
    auto p2_count = 0;

    while(g.running) {
        g.board.display();
        g.printPlayerInfo();

        auto start = clock();
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
        auto time = (clock() - start) / CLOCKS_PER_SEC;
        std::cout << "took " << time << " seconds to play " << move.toString() << "\n";
        p1_time += time;
        p1_count ++;

        if (fliped == -2) {
            g.board.display();
            std::cout << "avg p1_time: " << p1_time / p1_count << "\n";
            std::cout << "avg p2_time: " << p2_time / p2_count << "\n";
            g.endGame();
            break;
        }

        if (fliped == 0) {
            std::cout << "Bad move: " << move.toString() << " Must playa valid square, must flip at least 1 piecez\n";
            continue;
        }

        g.board.display();
        g.printPlayerInfo();

        start = clock();
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
        time = (clock() - start) / CLOCKS_PER_SEC; 
        std::cout << "took " <<  time << " seconds to play " << cpu_move.toString() << "\n";
        p2_time += time;
        p2_count ++;

        if (over == -2 ) {
            g.board.display();
            std::cout << "avg p1_time: " << p1_time / p1_count << "\n";
            std::cout << "avg p2_time: " << p2_time / p2_count << "\n";
            g.endGame();
            break;
        }
    }
}
