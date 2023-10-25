#include "game.cpp"
#include <iostream>

int main() {
    Player * p1 = new Player('o');
    Player * p2 = new Player('x');

    Game g = Game(p1, p2, ' ');

    while(g.running) {
        g.board->display();
        g.printPlayerInfo();

        std::cout << "enter thee coords, player: " << g.players[g.curr_idx]->piece << "\n";
        int i, j;
        scanf("%d %d", &i, &j);
        
        int fliped;

        if (Coord{i, j} == Coord{-2, -2}) {
            fliped = g.playBestMove();
        } else {
            fliped = g.play(Coord{i, j});
        }

        if (fliped == -2) {
            break;
        }
        
        if (fliped == 0) {
            std::cout << "Must playa valid square, must flip at least 1 piecez\n";
            continue;
        }
        
        g.board->display();
        g.printPlayerInfo();

        int over = g.playBestMove();
    }
}
