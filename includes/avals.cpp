#include "game.h"
#include "cpuplayer.h"
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <map>

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

float rollout2(const Game& g, const MinMaxCpuPlayer *const self) {
    auto game = g.clone();

    int turn = !(g.curr_idx);
    Coord sq = {-1, -1};
    int last_played = !turn;

    int p = 0;
    while(true){
        if (p == -2) break;

        Coord tmp = {-1, -1};
        std::array<Coord, 3> top_moves;
        int m_idx = 0;
        std::array<float, 3> top_weights;
        int w_idx = 0;

        for (int i = 0; i < GAME_N; i++) {
            for (int j = 0; j < GAME_N; j++) {
                if (game->board[Coord{i, j}] != game->board.empty_square_marker
                        || game->flipedFromMove(Coord{i, j}, game->curr_idx) == 0){
                    continue;
                }

                auto fit = 0;
                for (auto move: game->to_flip) {
                    fit += self->aval_matrix[move.row][move.col];
                }

                if (m_idx < 3) {
                    top_moves[m_idx] = {i, j}; m_idx++;
                    top_weights[w_idx] = fit; w_idx++;
                } else {
                    int idx = 0;
                    for (auto &val : top_weights) {
                        if (val < fit) {
                            top_weights[idx] = fit;
                            top_moves[idx] = {i, j};
                            break;
                        }
                        idx++;
                    }
                }
            }
        }

        if (m_idx != 0) {
            tmp = top_moves[rand() % m_idx];
        }

        // not sure why but it eliminates bad sub-trees i guess;
        if (tmp == sq && game->curr_idx != last_played && tmp == Coord{-1, -1}) {
            // error cases
            // if (game->players[0]->piece_count + game->players[1]->piece_count  < 20 && game->players[1]->piece_count != 0 && game->players[0]->piece_count != 0) {
            //     std::cout << "ended with " << game->players[0]->piece_count + game->players[1]->piece_count << " pieces\n";
            //     
            //     int c = 0;
            //     while (!game->flips.empty()) {
            //         std::cout << "did " << c << " undos\n";
            //         game->board.display();
            //         game->printPlayerInfo();
            //         game->undo();
            //         c++;
            //     } 
            //
            //     std::cout << "original:\n";
            //     g.board.display();
            //     g.printPlayerInfo();
            //
            //     std::cout << "tmp = " << tmp.toString() << "\n";
            //     std::cout << "sq = " << sq.toString() << "\n";
            //     std::cout << "p = " << p << "\n";
            //     std::cout << "m_idx " << m_idx << "\n";
            //     std::cout << "w_idx " << w_idx << "\n";
            //     std::cout << "curr_idx " << game->curr_idx << "\n";
            //     std::cout << "last_played " << last_played << "\n";
            //     exit(1);
            // }
            break;
        }
        sq = tmp;
        last_played = game->curr_idx;
        p = game->play(sq, true);
    }

    float dif = game->players[turn]->piece_count > game->players[!turn]->piece_count;
    if (dif > 0) {
        return 1;
    }
    if (dif == 0) {
        return 0;
    }
    return -1;
}

float rollout(const Game& g, const MinMaxCpuPlayer *const self) {
    auto game = g.clone();

    int turn = !(g.curr_idx);
    Coord sq = {-1, -1};
    int last_played = !turn;

    int p = 0;
    while(true){
        if (p == -2) break;

        Coord tmp = {-1, -1};
        std::vector<Coord> moves;
        // std::vector<int> weights;

        for (int i = 0; i < GAME_N; i++) {
            for (int j = 0; j < GAME_N; j++) {
                if (game->board[Coord{i, j}] != game->board.empty_square_marker
                        || game->flipedFromMove(Coord{i, j}, game->curr_idx) == 0){
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
        if (tmp == sq && game->curr_idx != last_played && tmp == Coord{-1, -1}) {
            break;
        }
        sq = tmp;
        last_played = game->curr_idx;
        p = game->play(sq, true);
    }

    int dif = game->players[turn]->piece_count - game->players[!turn]->piece_count;
    if (dif > 0) {
        return 1;
    }
    if (dif == 0) {
        return 0;
    }
    return -1;
}
