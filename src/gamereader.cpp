#include "game.cpp"
#include <functional>
#include <map>
#include <unistd.h>

class GameReader {
private:
    int avalMove(Coord c, Game game) {
        int fliped = game.play(c);
        // std::cout << "\n tryed " << c.row << " " << c.col << " and got " << fliped << "\n";
        return fliped;
    }

public:
    void playBestMove(Game * game) {
        std::map<int, Coord, std::greater<int>> moves;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int r = avalMove(Coord{i, j}, game->clone()); 
                // std::cout << "total pieces " << r << "\n";
                if (r == 0) {
                    continue;
                }
                moves[r] = Coord{i, j};
            }
        }

        int best_move_count = moves.begin()->first;

        std::cout << "\n playing " << moves[best_move_count].row << " " << moves[best_move_count].col << "\n";
        sleep(1);
        game->play(moves[best_move_count]);
    }
};

