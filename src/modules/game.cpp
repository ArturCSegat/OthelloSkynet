#include "../../includes/board.h"
#include "../../includes/game.h"
#include <chrono>
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

    if (player1->piece_count == 0 || player2->piece_count == 0) {
        player1->piece_count += 2;
        player2->piece_count += 2;
    }

    players[0] = std::move(player1);
    players[1] = std::move(player2);
    this->to_flip = {};
    this->to_flip.reserve(16);

    curr_idx = 0;
    running = true;
    play_count = 0;
}

bool Game::isValid(Coord move, int played_idx) const {

    return 
    (
    board.willFlipInVertical(move, players[curr_idx], players[!curr_idx])   ||
    board.willFlipInHorizontal(move, players[curr_idx], players[!curr_idx]) ||
    board.willFlipInDiegonalLR(move, players[curr_idx], players[!curr_idx]) ||
    board.willFlipInDiegonalRL(move, players[curr_idx], players[!curr_idx])
    ) && board[move] == board.empty_square_marker;
}

std::vector<Coord> Game::available() const {
    std::vector<Coord> v = {};
    for (int i = 0; i<GAME_N; i++) {
        for (int j = 0; j<GAME_N; j++) {
            if (isValid({i, j}, this->curr_idx)) {
                v.push_back({i, j});
            }
        }
    }
    return v;
}

int Game::flipedFromMove(Coord move, int player) {
    this->to_flip.clear();

    board.playVertical(move, players[curr_idx], players[!curr_idx], this->to_flip);
    board.playHorizontal(move, players[curr_idx], players[!curr_idx], this->to_flip);
    board.playDiegonalLR(move, players[curr_idx], players[!curr_idx], this->to_flip);
    board.playDiegonalRL(move, players[curr_idx], players[!curr_idx], this->to_flip);
    
    return this->to_flip.size();
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

    flipedFromMove(c, this->curr_idx);

    if (this->to_flip.size() == 0) {
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
    if (this->play_count == 0) {
        return;
    }

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
    this->play_count --;

    // std::cout << "undid: " << played.toString() << "\n";
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
            g->age_matrix[i][j] = age_matrix[i][j];
        }
    }

    g->curr_idx = curr_idx;
    // g->flips = flips;
    g->play_count = play_count;
    g->running = running;

    return g;
}

void Game::clone_into(Game& into) const {
    into.players[0]->piece_count = this->players[0]->piece_count;
    into.players[1]->piece_count = this->players[1]->piece_count;

    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            into.board[Coord{i, j}] = board[Coord{i, j}];
            into.age_matrix[i][j] = age_matrix[i][j];
        }
    }

    // g->flips = flips;
    into.curr_idx = curr_idx;
    into.play_count = play_count;
    into.running = running;
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

void Game::printPlayerInfo() const {
    std::cout << "turn: " << players[curr_idx]->piece << "\n";
    std::cout << "player " << players[0]->piece << ": " << players[0]->piece_count << "\n";
    std::cout << "player " << players[1]->piece << ": " << players[1]->piece_count << "\n";
}

bool Game::isOver() const {
    if (players[0]->piece_count + players[1]->piece_count == (GAME_N * GAME_N) || players[0]->piece_count == 0 || players[1]->piece_count == 0) {
        return true;
    }
    return false;

}
