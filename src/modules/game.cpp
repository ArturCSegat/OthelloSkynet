#include "../../includes/board.h"
#include "../../includes/game.h"
#include <iostream>
#include <vector>
#include <stack>
#include <math.h>

Game::Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square) {
    int sm_idx = (GAME_N - 1) / 2;
    int bg_idx = sm_idx + 1;

    board[Coord{sm_idx, sm_idx}] = player1->piece;
    board[Coord{bg_idx, bg_idx}] = player1->piece;
    board[Coord{sm_idx, bg_idx}] = player2->piece;
    board[Coord{bg_idx, sm_idx}] = player2->piece;
    player1->piece_count += 2;
    player2->piece_count += 2;

    players[0] = std::move(player1);
    players[1] = std::move(player2);

    curr_idx = 0;
    running = true;
}

std::vector<Coord> Game::flipedFromMove(Coord move, int player) const{
    std::vector<Coord> fill;

    board.playVertical(move, players[curr_idx], players[!curr_idx], fill);
    board.playHorizontal(move, players[curr_idx], players[!curr_idx], fill);
    board.playDiegonalLR(move, players[curr_idx], players[!curr_idx], fill);
    board.playDiegonalRL(move, players[curr_idx], players[!curr_idx], fill);
    
    return fill;
}

int Game::play(Coord c) {
    if (c.col < -1 || c.row < -1 || c.col > GAME_N - 1 || c.row > GAME_N - 1) {
        return 0;
    }

    Coord pass = Coord{-1, -1};

    if (c == pass) {
        curr_idx = !curr_idx;
        return -1;
    }

    if (board[c] != board.empty_square_marker) {
        // std::cout << "\n Invalid move, square already occupied. played by " << players[curr_idx]->piece << "\n\n";
        return 0;
    }

    std::vector<Coord> to_flip = flipedFromMove(c, this->curr_idx);

    if (to_flip.size() == 0) {
        return 0;
    }

    // std::cout << "played: " << c.toString() << "\n";
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            age_matrix[i][j] += 1;
        }
    }

    for (const Coord &c: to_flip) {
        board[c] = players[curr_idx]->piece;
        age_matrix[c.row][c.col] = 0;
    }
    this->play_count ++;

    board[c] = players[curr_idx]->piece;
    players[!curr_idx]->piece_count -= to_flip.size();
    players[curr_idx]->piece_count += to_flip.size() + 1;
    curr_idx = !curr_idx;

    if (players[0]->piece_count + players[1]->piece_count == (GAME_N * GAME_N) || players[0]->piece_count == 0 || players[1]->piece_count == 0) {
        to_flip.push_back(c);
        this->flips.push(to_flip);
        return -2;
    }

    to_flip.push_back(c);
    this->flips.push(to_flip);

    return to_flip.size() - 1;
}

void Game::undo() {
    auto did = this->flips.top();
    this->flips.pop();

    Coord played = did[did.size() - 1];

    for (auto& c: did) {
        this->board[c] = this->players[this->curr_idx]->piece;
        age_matrix[c.row][c.col] = 10; // bigger then cpuplayer's MAX DETPH
    }

    this->players[this->curr_idx]->piece_count += did.size() - 1;
    this->players[!this->curr_idx]->piece_count -= did.size();

    this->board[played] = this->board.empty_square_marker;

    this->curr_idx =! this->curr_idx;

    // std::cout << "undone: " << played.toString() << "\n";
}

std::unique_ptr<Game> Game::clone() const{
    auto p1 = std::make_unique<Player>(Player(players[0]->piece));
    p1->piece_count = players[0]->piece_count;
    auto p2 =  std::make_unique<Player>(Player(players[1]->piece));
    p2->piece_count = players[1]->piece_count;
    char df = board.empty_square_marker;

    auto g = std::make_unique<Game>(Game(std::move(p1), std::move(p2), df));

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            g->board[Coord{i, j}] = board[Coord{i, j}];
        }
    }

    g->curr_idx = curr_idx;

    return g;
}

void Game::endGame() {
    printPlayerInfo();

    int winner_idx = players[1]->piece_count > players[0]->piece_count;
    int reverse = players[0]->piece_count > players[1]->piece_count;


    if (winner_idx == reverse) {
        std::cout << "\n\n The Grame Drawed\n\n";
        return;
    }

    std::cout << "\n\n The Winner of game " << this << " is " << players[winner_idx]->piece << "\n\n";
    running = false;
}

void Game::printPlayerInfo(){
    std::cout << "player " << players[0]->piece << ": " << players[0]->piece_count << "\n";
    std::cout << "player " << players[1]->piece << ": " << players[1]->piece_count << "\n";
}
