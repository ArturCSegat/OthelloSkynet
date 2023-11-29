#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#define GAME_COUNT 100

int main() {

    int p1_w_c = 0;
    int p2_w_c = 0;
    int draws = 0;
    srand (static_cast <unsigned> (time(0)));
    
    for (int i = 0; i  < GAME_COUNT; i++) {

        auto p1 = std::make_unique<CpuPlayer>(CpuPlayer('o'));
        auto p2 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('x'));

        auto g = Game(std::move(p1), std::move(p2), ' ');

        while(g.running) {
            // g.board.display();
            // g.printPlayerInfo();

            Coord move = g.players[g.curr_idx]->choseSquare(g);

            int fliped;

            fliped = g.play(move);

            if (fliped == -2) {
                // g.board.display();
                g.endGame();
                // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                int winner_idx = g.players[1]->piece_count > g.players[0]->piece_count;
                int draw  = g.players[1]->piece_count == g.players[0]->piece_count;
                
                if (draw) {
                    draws++;
                } else if (winner_idx == 1) {
                    p2_w_c++;
                } else if (winner_idx == 0) {
                    p1_w_c++;
                }
                break;
            }

            if (fliped == 0) {
                std::cout << "Must playa valid square, must flip at least 1 piecez\n";
                continue;
            }

            // g.board.display();
            // g.printPlayerInfo();

            Coord cpu_move = g.players[g.curr_idx]->choseSquare(g);

            if(cpu_move == move) {
                std::cout << "Both skipped, no more moves availabe\n";
                g.endGame();
                int winner_idx = g.players[1]->piece_count > g.players[0]->piece_count;
                int draw  = g.players[1]->piece_count == g.players[0]->piece_count;
                
                if (draw) {
                    draws++;
                } else if (winner_idx == 1) {
                    p2_w_c++;
                } else if (winner_idx == 0) {
                    p1_w_c++;
                }
                break;
            }

            int over = g.play(cpu_move);

            if (over == -2 ) {
                // g.board.display();
                g.endGame();
                // std::this_thread::sleep_for(std::chrono::milliseconds(500));
                int winner_idx = g.players[1]->piece_count > g.players[0]->piece_count;
                int draw  = g.players[1]->piece_count == g.players[0]->piece_count;
                
                if (draw) {
                    draws++;
                } else if (winner_idx == 1) {
                    p2_w_c++;
                } else if (winner_idx == 0) {
                    p1_w_c++;
                }
                break;
            }
        }
    }
    std::cout << "p1 win count: " << p1_w_c << "\n";
    std::cout << "p2 win count: " << p2_w_c << "\n";
    std::cout << "draws: " << draws << "\n";
}
