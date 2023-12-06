#include "../../includes/cpuplayer.h"
#include "../../includes/game.h"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

#define MAX_DEPTH 2

BadCpuPlayer::BadCpuPlayer(char p) : Player(p) {}

Coord BadCpuPlayer::choseSquare(Game& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker) {
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}, game.curr_idx).size(); 
            if (flipped == 0) {
                continue;
            }
            
            moves[flipped] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];

    std::cout << "Playing " << best_move.toString() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return best_move;

}

CpuPlayer::CpuPlayer(char p): Player(p){
    aval_rows[0] = 2.5;
    aval_rows[1] = 0.5;
    aval_rows[2] = 1;
    aval_rows[3] = 1.5;
    aval_rows[4] = 1.5;
    aval_rows[5] = 1;
    aval_rows[6] = 0.5;
    aval_rows[7] = 2.5;

    aval_cols[0] = 2.5;
    aval_cols[1] = 0.5;
    aval_cols[2] = 1;
    aval_cols[3] = 1.5;
    aval_cols[4] = 1.5;
    aval_cols[5] = 1;
    aval_cols[6] = 0.5;
    aval_cols[7] = 2.5;
}

Coord CpuPlayer::choseSquare(Game& game) {
    std::map<float, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker){
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}, game.curr_idx).size(); 
            if (flipped == 0) {
                continue;
            }
            
            float positional_factor = 1.5 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-1.5))); 
            positional_factor = 2;
            float fit = std::pow((aval_rows[i] * aval_cols[j]), positional_factor) + flipped;

            moves[fit] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        return Coord{-1, -1};
    }

    Coord best_move = moves[moves.rbegin()->first];
    return best_move;
}



BetterCpuPlayer::BetterCpuPlayer(char p) : CpuPlayer(p) {};

Coord BetterCpuPlayer::choseSquare(Game& game) {
    std::map<float, Coord> moves;
    int c = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).size() == 0){
                continue;
            }
            moves[avaliateMoveTillEnd(Coord{i, j}, std::move(game.clone()))] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        // std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    // for (const auto &[fit, move]: moves) {
    //     std::cout << "move: " << move.toString() << " value: " << fit << "\n";
    // }

    Coord best_move = moves[moves.rbegin()->first];

    // std::cout << "Playing " << best_move.toString() << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return best_move;
}

int BetterCpuPlayer::avaliateMoveTillEnd(Coord move, std::unique_ptr<Game> game) {
    // std::cout << "move: " << move.toString() << " avaliateMoveTillEnd\n";
    game->play(move);
    Coord sq;
    do{
        Coord tmp = CpuPlayer::choseSquare(*game);
        // not sure why but it eliminates bad sub-trees i guess;
        if (tmp == sq) {
            break;
        }
        sq = tmp;
    }while(game->play(sq) != -2);

    int this_idx = game->players[1]->piece == this->piece;

    return game->players[this_idx]->piece_count - game->players[!this_idx]->piece_count;
}



MaybeEvenBetterCpuPlayer::MaybeEvenBetterCpuPlayer(char p) : BetterCpuPlayer(p) {};

Coord MaybeEvenBetterCpuPlayer::choseSquare(Game& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).size() == 0){
                continue;
            }
            moves[avaliateShallowTreeTillEnd(Coord{i, j}, game.clone())] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    // for (const auto &[fit, move]: moves) {
    //     std::cout << "move: " << move.toString() << " value: " << fit << "\n";
    // }

    Coord best_move = moves[moves.rbegin()->first];

    // std::cout << "Playing " << best_move.toString() << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    return best_move;
}

int MaybeEvenBetterCpuPlayer::avaliateShallowTreeTillEnd(Coord move, std::unique_ptr<Game> game) {
    game->play(move);
    
    int aval_sum;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(game->board[Coord{i, j}] != game->board.empty_square_marker
                    || game->flipedFromMove(Coord{i, j}, game->curr_idx).size() == 0) {
                continue;
            }
            int fit = avaliateMoveTillEnd(Coord{i, j}, game->clone());
            aval_sum += fit;
        }
    }

    return aval_sum;
}

MinMaxCpuPlayer::MinMaxCpuPlayer(char p) : BetterCpuPlayer(p) {};

Coord MinMaxCpuPlayer::choseSquare(Game& game) {
    float max_aval = -9999999;
    auto max_move = Coord{-1, -1};
    
    auto p1 = game.players[0]->piece_count;
    auto p2 = game.players[1]->piece_count;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).size() == 0){
                continue;
            }
            
            float fit = Max(game, Coord{i, j}, -999, 999, 0);
            game.undo();

            if (fit > max_aval) {
                max_aval = fit;
                max_move = Coord{i, j};
            }
        }
    }

    auto p12 = game.players[0]->piece_count;
    auto p22 = game.players[1]->piece_count;
    
    if (p1 != p12 || p2 != p22) {
        std::cout << "\nfuck\n";
        std::cout << "p1 :" << p1 << " p12: " << p12 << "\n";
        std::cout << "p2 :" << p2 << " p22: " << p22 << "\n";
        std::cout << "\nfuck\n";
        exit(1);
    }

    return max_move;
}

float MinMaxCpuPlayer::Max(Game& game, Coord move, float alpha, float beta, int depth) {
    // std::cout << "Max: move: " << move.toString() << ", depth: " << depth << "\n";

    // std::cout << "MAx turn: " << game.curr_idx <<  "depth: " << depth <<"\n";
    float max_aval = -9999999;
    float aval;
    int r = game.play(move);


    if (r == -2 || depth == MAX_DEPTH) {
        return game.playerAval(CpuPlayer::aval_rows, CpuPlayer::aval_cols);
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).size() == 0){
                continue;
            }
            aval = Min(game, Coord{i, j}, alpha, beta, depth + 1);
            game.undo();
            alpha = std::max(alpha, aval);
            if (aval > max_aval) {
                max_aval = aval;
            }
            
            if (beta <= alpha) {
                goto exit;
            }
        }
    }
    exit:
    return max_aval;
}


float MinMaxCpuPlayer::Min(Game& game, Coord move, float alpha, float beta, int depth) {
    // std::cout << "Min: move: " << move.toString() << ", depth: " << depth << "\n";

    // std::cout << "Min turn: " << game.curr_idx <<  "depth: " << depth <<"\n";
    float max_aval = 9999999;
    float aval;
    int r = game.play(move);

    if (r == -2 || depth == MAX_DEPTH) {
        return game.playerAval(CpuPlayer::aval_rows, CpuPlayer::aval_cols);
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}, game.curr_idx).size() == 0){
                continue;
            }
            aval = Max(game, Coord{i, j}, alpha, beta, depth + 1);
            game.undo();
            if (aval < max_aval) {
                max_aval = aval;
            }
            
            beta = std::min(beta, aval);
            if (beta <= alpha) {
                goto exit;
            }
        }
    }
    exit:
    return max_aval;
}
