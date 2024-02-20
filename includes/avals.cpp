#include "game.h"
#include "cpuplayer.h"
#include "player.h"
#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <cmath>
#include <map>

float aval(Game& game, const MinMaxCpuPlayer *const self) {
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

float aval2(Game& game, const MinMaxCpuPlayer *const self) {
    int this_idx = game.players[1]->piece == self->piece;
    int op_idx = !(bool)this_idx;

    float this_aval = 1;
    float op_aval = 1;
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

            if (space == game.players[this_idx]->piece) {
                this_aval += aval;
                continue;
            }
            op_aval += aval;
        }
    }

    int i = 0;
    for (auto &m: modifiers) {
        if(list[i] == game.players[this_idx]->piece) {
            this_aval *= m;
            continue;
        }
        op_aval *= m;
        i++;
    }
    
    // std::cout << p1_aval << " - " << p0_aval << "\n";
    return this_aval - op_aval;
}


float rollout(Game& g, const MinMaxCpuPlayer *const self) {
    auto p1 = std::make_unique<Player>(Player('o'));
    auto p2 = std::make_unique<Player>(Player('x'));
    auto game = Game(std::move(p1), std::move(p2), ' ');

    g.clone_into(game);

    int turn = !(g.curr_idx);
    Coord sq = {-1, -1};
    int last_played = !turn;

    int p = 0;
    std::vector<Coord> moves;
    while(true){
        if (p == -2) break;

        Coord tmp = {-1, -1};
        moves.clear();
        // std::vector<int> weights;

        for (int i = 0; i < GAME_N; i++) {
            for (int j = 0; j < GAME_N; j++) {
                if (game.board[Coord{i, j}] != game.board.empty_square_marker
                        || game.isValid(Coord{i, j}, game.curr_idx) == false){
                    continue;
                }

                moves.push_back({i, j});
                // int m = i * 10 + j;
                // switch (m) {
                //    case 11:
                //    case 16:
                //    case 61:
                //    case 66:
                //        weights.push_back(1);
                //        break;
                //     default:
                //        weights.push_back(10);
                //        break;
                // }
                
                
            }
        }

        if (!moves.empty()) {
            // Calculate total weight
            // int totalWeight = 0;
            // for (auto w: weights) {
            //     totalWeight += w;
            // }
            //
            // // Generate a random number in the range [0, totalWeight)
            // int randNum = rand() % totalWeight;
            //
            // // Choose move based on weighted random selection
            // int cumulativeWeight = 0;
            // for (size_t i = 0; i < moves.size(); ++i) {
            //     cumulativeWeight += weights[i];
            //     if (randNum < cumulativeWeight) {
            //         tmp = moves[i];
            //         break;
            //     }
            // }        
            tmp = moves[rand() % moves.size()];
        }

        // not sure why but it eliminates bad sub-trees i guess;
        if (tmp == sq && game.curr_idx != last_played && tmp == Coord{-1, -1}) {
            break;
        }
        sq = tmp;
        last_played = game.curr_idx;
        p = game.play(sq);
    }

    int dif = game.players[turn]->piece_count - game.players[!turn]->piece_count;
    if (dif > 0) {
        return 1;
    }
    if (dif == 0) {
        return 0;
    }
    return -1;
}
