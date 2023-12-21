#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <math.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

float aval(const Game& game, const MinMaxCpuPlayer *const self) {
    float p0_aval = 1;
    float p1_aval = 1;
    float aval;
    
    std::vector<float> modifiers;
    std::vector<char> list;

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            char space = game.board[Coord{i, j}];

            if (space == game.board.empty_square_marker
                ||game.age_matrix[i][j] > self->max_depth
                ) {
                continue;
            }

            aval = self->aval_matrix[i][j] * (9 - (game.age_matrix[i][j] + 1));
            int bg_idx = GAME_N - 1;

            if (
                (Coord{i, j} == Coord{1, bg_idx - 1}) ||
                (Coord{i, j} == Coord{1, 1}) ||
                (Coord{i, j} == Coord{bg_idx - 1, 1}) ||
                (Coord{i, j} == Coord{bg_idx - 1, bg_idx - 1}) ||

                (Coord{i, j} == Coord{0, 0}) ||
                (Coord{i, j} == Coord{0, bg_idx}) ||
                (Coord{i, j} == Coord{bg_idx, 0}) ||
                (Coord{i, j} == Coord{bg_idx, bg_idx}) ||

                (Coord{i, j} == Coord{0, 1}) ||
                (Coord{i, j} == Coord{0, bg_idx - 1}) ||
                (Coord{i, j} == Coord{1, 0}) ||
                (Coord{i, j} == Coord{1, bg_idx}) ||

                (Coord{i, j} == Coord{bg_idx - 1, 0}) ||
                (Coord{i, j} == Coord{bg_idx - 1, bg_idx}) ||

                (Coord{i, j} == Coord{bg_idx, 1}) ||
                (Coord{i, j} == Coord{bg_idx, bg_idx - 1})
                ) {
                
                modifiers.push_back(pow(aval, 2));
                list.push_back(space);
                continue;
            }

            if (space == game.players[0]->piece) {
                p0_aval += aval;
                continue;
            }
            p1_aval += aval;
        }
    }

    int i = 0;
    for (auto &m: modifiers) {
        if(list[i] == game.players[0]->piece) {
            p0_aval *= m;
            continue;
        }
        p1_aval *= m;
        i++;
    }
    
    // std::cout << p1_aval << " - " << p0_aval << "\n";
    return p1_aval - p0_aval;
}

float aval2(const Game& game, const MinMaxCpuPlayer *const self) {
    float p0_aval = 1;
    float p1_aval = 1;
    float aval;
    
    std::vector<float> modifiers;
    std::vector<char> list;

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            char space = game.board[Coord{i, j}];

            if (space == game.board.empty_square_marker
                ||game.age_matrix[i][j] > self->max_depth
                ) {
                continue;
            }

            aval = self->aval_matrix[i][j] * (9 - (game.age_matrix[i][j] + 1));
            int bg_idx = GAME_N - 1;

            if (
                (Coord{i, j} == Coord{1, bg_idx - 1}) ||
                (Coord{i, j} == Coord{1, 1}) ||
                (Coord{i, j} == Coord{bg_idx - 1, 1}) ||
                (Coord{i, j} == Coord{bg_idx - 1, bg_idx - 1}) ||

                (Coord{i, j} == Coord{0, 0}) ||
                (Coord{i, j} == Coord{0, bg_idx}) ||
                (Coord{i, j} == Coord{bg_idx, 0}) ||
                (Coord{i, j} == Coord{bg_idx, bg_idx}) ||

                (Coord{i, j} == Coord{0, 1}) ||
                (Coord{i, j} == Coord{0, bg_idx - 1}) ||
                (Coord{i, j} == Coord{1, 0}) ||
                (Coord{i, j} == Coord{1, bg_idx}) ||

                (Coord{i, j} == Coord{bg_idx - 1, 0}) ||
                (Coord{i, j} == Coord{bg_idx - 1, bg_idx}) ||

                (Coord{i, j} == Coord{bg_idx, 1}) ||
                (Coord{i, j} == Coord{bg_idx, bg_idx - 1})
                ) {
                
                modifiers.push_back(aval);
                list.push_back(space);
                continue;
            }

            if (space == game.players[0]->piece) {
                p0_aval += aval;
                continue;
            }
            p1_aval += aval;
        }
    }

    int i = 0;
    for (auto &m: modifiers) {
        if(list[i] == game.players[0]->piece) {
            p0_aval *= m;
            continue;
        }
        p1_aval *= m;
        i++;
    }
    
    // std::cout << p1_aval << " - " << p0_aval << "\n";
    return p1_aval - p0_aval;
}

int main() {
    srand (static_cast <unsigned> (time(0)));
    auto p1 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('o', aval, 5));
    auto p2 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('x', aval2, 5));

    auto g = Game(std::move(p1), std::move(p2), ' ');

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
            std::cout << "Must playa valid square, must flip at least 1 piecez\n";
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
