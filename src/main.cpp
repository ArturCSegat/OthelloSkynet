#include "../includes/game.h"
#include "../includes/cpuplayer.h"
#include <math.h>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

float aval(const Game& game, float aval_matrix[8][8]) {
    float p0_aval = 1;
    float p1_aval = 1;
    float aval;
    
    std::vector<float> modifiers;
    std::vector<char> list;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char space = game.board[Coord{i, j}];

            if (space == game.board.empty_square_marker ||
                game.age_matrix[i][j] > 7
                ) {
                continue;
            }

            aval = aval_matrix[i][j] * (9 - (game.age_matrix[i][j] + 1));

            if (
                (Coord{i, j} == Coord{1, 6}) ||
                (Coord{i, j} == Coord{1, 1}) ||
                (Coord{i, j} == Coord{6, 1}) ||
                (Coord{i, j} == Coord{6, 6}) ||

                (Coord{i, j} == Coord{0, 0}) ||
                (Coord{i, j} == Coord{0, 7}) ||
                (Coord{i, j} == Coord{7, 0}) ||
                (Coord{i, j} == Coord{7, 7}) ||

                (Coord{i, j} == Coord{0, 1}) ||
                (Coord{i, j} == Coord{0, 6}) ||
                (Coord{i, j} == Coord{1, 0}) ||
                (Coord{i, j} == Coord{1, 7}) ||

                (Coord{i, j} == Coord{6, 0}) ||
                (Coord{i, j} == Coord{6, 7}) ||

                (Coord{i, j} == Coord{7, 1}) ||
                (Coord{i, j} == Coord{7, 6})
                ) {
                
                modifiers.push_back(pow(aval, 3));
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

    return p1_aval - p0_aval;
}

float aval2(const Game& game, float aval_matrix[8][8]) {
    float p0_aval = 1;
    float p1_aval = 1;
    float aval;
    int count0 = 0;
    int count1 = 0;
    
    std::vector<float> modifiers;
    std::vector<char> list;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char space = game.board[Coord{i, j}];

            if (space == game.board.empty_square_marker ||
                game.age_matrix[i][j] > 7
                ) {
                continue;
            }

            aval = aval_matrix[i][j] * (9 - (game.age_matrix[i][j] + 1));

            if (
                (Coord{i, j} == Coord{1, 6}) ||
                (Coord{i, j} == Coord{1, 1}) ||
                (Coord{i, j} == Coord{6, 1}) ||
                (Coord{i, j} == Coord{6, 6}) ||

                (Coord{i, j} == Coord{0, 0}) ||
                (Coord{i, j} == Coord{0, 7}) ||
                (Coord{i, j} == Coord{7, 0}) ||
                (Coord{i, j} == Coord{7, 7}) ||

                (Coord{i, j} == Coord{0, 1}) ||
                (Coord{i, j} == Coord{0, 6}) ||
                (Coord{i, j} == Coord{1, 0}) ||
                (Coord{i, j} == Coord{1, 7}) ||

                (Coord{i, j} == Coord{6, 0}) ||
                (Coord{i, j} == Coord{6, 7}) ||

                (Coord{i, j} == Coord{7, 1}) ||
                (Coord{i, j} == Coord{7, 6})
                ) {
                
                modifiers.push_back(pow(aval, 3));
                list.push_back(space);
                continue;
            }

            if (space == game.players[0]->piece) {
                p0_aval += aval;
                count0 ++;
                continue;
            }
            p1_aval += aval;
            count1 ++;
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

    return p1_aval - p0_aval;
}

int main() {
    srand (static_cast <unsigned> (time(0)));
    auto p1 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('o', aval));
    auto p2 = std::make_unique<MinMaxCpuPlayer>(MinMaxCpuPlayer('x', aval2));

    auto g = Game(std::move(p2), std::move(p1), ' ');

    while(g.running) {
        g.board.display();
        g.printPlayerInfo();

        Coord move = g.players[g.curr_idx]->choseSquare(g);

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
