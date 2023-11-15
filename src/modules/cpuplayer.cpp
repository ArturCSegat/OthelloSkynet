#include "../../includes/cpuplayer.h"
#include "../../includes/game.h"
#include <cmath>
#include <ctime>
#include <map>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

BadCpuPlayer::BadCpuPlayer(char p) : Player(p) {}

Coord BadCpuPlayer::choseSquare(const Game& game) {
    std::map<int, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker) {
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}).size(); 
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

Coord CpuPlayer::choseSquare(const Game& game) {
    std::map<float, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker){
                continue;
            }
            int flipped = game.flipedFromMove(Coord{i, j}).size(); 
            if (flipped == 0) {
                continue;
            }
            
            float fit = std::pow((aval_rows[i] * aval_cols[j]), 2) + flipped;

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

Coord BetterCpuPlayer::choseSquare(const Game& game) {
    std::map<float, Coord> moves;
    int c = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}).size() == 0){
                continue;
            }
            moves[avaliateMoveTillEnd(Coord{i, j}, std::move(game.clone()))] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        // std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    for (const auto &[fit, move]: moves) {
        std::cout << "move: " << move.toString() << " value: " << fit << "\n";
    }

    Coord best_move = moves[moves.rbegin()->first];

    // std::cout << "Playing " << best_move.toString() << "\n";
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return best_move;
}

float BetterCpuPlayer::avaliateMoveTillEnd(Coord move, std::unique_ptr<Game> game) {
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

    if (game->players[this_idx]->piece_count < game->players[!this_idx]->piece_count) {
        return (game->players[this_idx]->piece_count / (float)(play_count + 1)) + 0.1;
    }
    
    return game->players[this_idx]->play_count;
}



MaybeEvenBetterCpuPlayer::MaybeEvenBetterCpuPlayer(char p) : BetterCpuPlayer(p) {};

Coord MaybeEvenBetterCpuPlayer::choseSquare(const Game& game) {
    std::map<float, Coord> moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (game.board[Coord{i, j}] != game.board.empty_square_marker
                    || game.flipedFromMove(Coord{i, j}).size() == 0){
                continue;
            }
            moves[avaliateShallowTreeTillEnd(Coord{i, j}, game.clone())] = Coord{i, j};
        }
    }

    if (moves.empty()) {
        std::cout << "Skipping my turn\n";
        return Coord{-1, -1};
    }
    
    for (const auto &[fit, move]: moves) {
        std::cout << "move: " << move.toString() << " value: " << fit << "\n";
    }

    Coord best_move = moves[moves.rbegin()->first];

    std::cout << "Playing " << best_move.toString() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(750));
    return best_move;
}

int MaybeEvenBetterCpuPlayer::avaliateShallowTreeTillEnd(Coord move, std::unique_ptr<Game> game) {
    game->play(move);
    
    int winning_count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(game->board[Coord{i, j}] != game->board.empty_square_marker
                    || game->flipedFromMove(Coord{i, j}).size() == 0) {
                continue;
            }
            float fit = avaliateMoveTillEnd(Coord{i, j}, game->clone());
            if ((int)fit == fit) {
                winning_count++;
            }
        }
    }

    return winning_count;
}
