#include "game.cpp"
#include <cstdio>
#include <iostream>
#include <vector>

int main() {
    Player * p1 = new Player('o');
    Player * p2 = new Player('x');

    Game g = Game(p1, p2, ' ');

    while(true) {
        g.board->display();

        std::cout << "enter thee coords, player: " << g.players[g.curr_idx]->piece << "\n";
        int i, j;
        scanf("%d %d", &i, &j);

        g.play(Coord{i, j});
    }
}
