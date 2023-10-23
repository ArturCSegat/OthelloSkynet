#include "board.cpp"
#include <cmath>
#include <map>
#include <unordered_map>
#include <thread>
#include <chrono>


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

    int play(Coord c) {
        
        Coord pass = Coord{-1, -1};

        if (c == pass) {
            curr_idx = !curr_idx;
            return -1;
        }

        if ((*board)[c] != board->empty_square_marker) {
            // std::cout << "\n Invalid move, square already occupied. played by " << players[curr_idx]->piece << "\n\n";
            return 0;
        }

        int old = players[curr_idx]->piece_count;

        board->playVertical(c, players[curr_idx], players[!curr_idx]);
        board->playHorizontal(c, players[curr_idx], players[!curr_idx]);
        board->playDiegonalLR(c, players[curr_idx], players[!curr_idx]);
        board->playDiegonalRL(c, players[curr_idx], players[!curr_idx]);

        if (players[curr_idx]->piece_count == old) {
            // std::cout << "\n Invalid move, must flip at least 1 piece. played by " << players[curr_idx]->piece << "\n\n";
            return 0;
        }

        (*board)[c] = players[curr_idx]->piece;
        curr_idx = !curr_idx;
        return players[!curr_idx]->piece_count - old;
    }

    Game clone() {
        Player * p1 = new Player(players[0]->piece);
        p1->piece_count = players[0]->piece_count;
        Player * p2 = new Player(players[1]->piece);
        p2->piece_count = players[1]->piece_count;
        char df = board->empty_square_marker;

        Game g = Game(p1, p2, df);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                g.board->board[i][j] = (*board)[Coord{i, j}];
            }
        }

        g.curr_idx = curr_idx;

        return g;
    }

    void playBestMove() {
        std::map<int, Coord> moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int r = this->clone().play(Coord{i, j}); 
                if (r == 0) {
                    continue;
                }
                moves[r] = Coord{i, j};
            }
        }

        Coord best_move = moves[moves.rbegin()->first];
        std::cout << "\n playing " << best_move.toString() << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->play(best_move);
    }

};

