#include "board.cpp"
#include <algorithm>
#include <map>
#include <memory>
#include <thread>
#include <chrono>


class Game{
public:
    Board * board;
    Player * players[2];
    int curr_idx;
    bool running;
    
    Game(Player * player1, Player * player2, char default_square) {
        board = new Board(default_square);

        (*board)[Coord{3, 4}] = player1->piece;
        (*board)[Coord{4, 3}] = player1->piece;
        (*board)[Coord{3, 3}] = player2->piece;
        (*board)[Coord{4, 4}] = player2->piece;
        player1->piece_count += 2;
        player2->piece_count += 2;

        players[0] = player1;
        players[1] = player2;
        
        curr_idx = 0;
        running = true;
    }

    int play(Coord c) {
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
        players[curr_idx]->piece_count++;
        curr_idx = !curr_idx;
        
        if (players[!curr_idx]->piece_count + players[curr_idx]->piece_count == 64) {
            this->endGame();
            return -2;
        }

        return players[!curr_idx]->piece_count - old;
    }

    std::unique_ptr<Game> clone() {
        Player * p1 = new Player(players[0]->piece);
        p1->piece_count = players[0]->piece_count;
        Player * p2 = new Player(players[1]->piece);
        p2->piece_count = players[1]->piece_count;
        char df = board->empty_square_marker;

        std::unique_ptr<Game> g = std::make_unique<Game>(Game(p1, p2, df));

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                g->board->board[i][j] = (*board)[Coord{i, j}];
            }
        }

        g->curr_idx = curr_idx;

        return g;
    }

    int playBestMove() {
        std::map<int, Coord> moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if ((*board)[Coord{i, j}] != this->board->empty_square_marker) {
                    continue;
                }
                int r = this->clone()->play(Coord{i, j}); 
                if (r == 0) {
                    continue;
                }
                moves[r] = Coord{i, j};
            }
        }
        
        if (moves.empty()) {
            std::cout << "\n skiping my turn\n\n";
            return this->play(Coord{-1, -1});
        }
        
        Coord best_move = moves[moves.rbegin()->first];
        std::cout << "\n playing " << best_move.toString() << " as " << players[curr_idx]->piece << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        return this->play(best_move);
    }

    void endGame() {
        printPlayerInfo();

        Player * winner;
        if (players[0]->piece_count > players[1]->piece_count) {
            winner = players[0];
        } else if (players[0]->piece_count == players[1]->piece_count) {
            winner = NULL;
        } else {
            winner = players[1];
        }

        if (winner == NULL) {
            std::cout << "\n\n The Grame Drawed\n\n";
            return;
        }

        std::cout << "\n\n The Winner of game " << this << " is " << winner->piece << "\n\n";
        running = false;
    }

    void printPlayerInfo(){
        std::cout << "player " << players[0]->piece << ": " << players[0]->piece_count << "\n";
        std::cout << "player " << players[1]->piece << ": " << players[1]->piece_count << "\n";
    }

};
