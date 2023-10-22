#include <cstddef>
#include <cstdio>
#include <vector>
#include <iostream>
#include "board.cpp"


class Game{
public:
    Board * board;
    Player * players[2];
    int curr_idx;
    
    Game(Player * player1, Player * player2, char default_square) {
        board = new Board(default_square);

        (*board)[Coord{3, 4}] = player1->piece;
        (*board)[Coord{4, 3}] = player1->piece;
        (*board)[Coord{3, 3}] = player2->piece;
        (*board)[Coord{4, 4}] = player2->piece;

        players[0] = player1;
        players[1] = player2;

        curr_idx = 0;
    }

    void play(Coord c) {

        int old = players[0]->piece_count;

        board->playVertical(c, players[curr_idx], players[!curr_idx]);
        board->playHorizontal(c, players[curr_idx], players[!curr_idx]);
        board->playDiegonalLR(c, players[curr_idx], players[!curr_idx]);
        board->playDiegonalRL(c, players[curr_idx], players[!curr_idx]);

        if (players[0]->piece_count == old) {
            std::cout << "\n Invalid move, must flip at least 1 piece \n\n";
            return;
        }

        (*board)[c] = players[curr_idx]->piece;
        curr_idx = !curr_idx;
    }
};

