#include "../../includes/board.h"
#include "../../includes/game.h"
#include <iostream>
#include <vector>

Game::Game(std::unique_ptr<Player> player1, std::unique_ptr<Player> player2, char default_square) {
    board = std::make_unique<Board>(Board(default_square));

    (*board)[Coord{3, 4}] = player1->piece;
    (*board)[Coord{4, 3}] = player1->piece;
    (*board)[Coord{3, 3}] = player2->piece;
    (*board)[Coord{4, 4}] = player2->piece;
    player1->piece_count += 2;
    player2->piece_count += 2;

    players[0] = std::move(player1);
    players[1] = std::move(player2);

    curr_idx = 0;
    running = true;
}

std::vector<Coord> Game::flipedFromMove(Coord move) {
    std::vector<Coord> fill;
    fill.reserve(24);

    board->playVertical(move, players[curr_idx], players[!curr_idx], fill);
    board->playHorizontal(move, players[curr_idx], players[!curr_idx], fill);
    board->playDiegonalLR(move, players[curr_idx], players[!curr_idx], fill);
    board->playDiegonalRL(move, players[curr_idx], players[!curr_idx], fill);
    
    return fill;
}

int Game::play(Coord c) {
    if (c.col < -1 || c.row < -1 || c.col > 7 || c.row > 7) {
        return 0;
    }

    Coord pass = Coord{-1, -1};

    if (c == pass) {
        curr_idx = !curr_idx;
        return -1;
    }

    if ((*board)[c] != board->empty_square_marker) {
        // std::cout << "\n Invalid move, square already occupied. played by " << players[curr_idx]->piece << "\n\n";
        return 0;
    }

    std::vector<Coord> to_flip = flipedFromMove(c);

    if (to_flip.size() == 0) {
        return 0;
    }

    for (int i = 0; i < to_flip.size(); i++) {
        (*board)[to_flip[i]] = players[curr_idx]->piece;
    }

    (*board)[c] = players[curr_idx]->piece;
    players[!curr_idx]->piece_count -= to_flip.size();
    players[curr_idx]->piece_count += to_flip.size() + 1;
    curr_idx = !curr_idx;

    if (players[0]->piece_count + players[1]->piece_count == 64 || players[0]->piece_count == 0 || players[1]->piece_count == 0) {
        return -2;
    }

    return to_flip.size();
}

std::unique_ptr<Game> Game::clone() {
    std::unique_ptr<Player> p1 = std::make_unique<Player>(Player(players[0]->piece));
    p1->piece_count = players[0]->piece_count;
    std::unique_ptr<Player> p2 =  std::make_unique<Player>(Player(players[1]->piece));
    p2->piece_count = players[1]->piece_count;
    char df = board->empty_square_marker;

    std::unique_ptr<Game> g = std::make_unique<Game>(Game(std::move(p1), std::move(p2), df));

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            g->board->board[i][j] = (*board)[Coord{i, j}];
        }
    }

    g->curr_idx = curr_idx;

    return g;
}

void Game::endGame() {
    printPlayerInfo();

    std::unique_ptr<Player> * winner;
    if (players[0]->piece_count > players[1]->piece_count) {
        winner = &players[0];
    } else if (players[0]->piece_count == players[1]->piece_count) {
        winner = NULL;
    } else {
        winner = &players[1];
    }

    if (winner == NULL) {
        std::cout << "\n\n The Grame Drawed\n\n";
        return;
    }

    std::cout << "\n\n The Winner of game " << this << " is " << (*winner)->piece << "\n\n";
    running = false;
}

void Game::printPlayerInfo(){
    std::cout << "player " << players[0]->piece << ": " << players[0]->piece_count << "\n";
    std::cout << "player " << players[1]->piece << ": " << players[1]->piece_count << "\n";
}