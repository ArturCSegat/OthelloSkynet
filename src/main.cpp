#include "game.cpp"
#include <cstdio>
#include <vector>

int main() {
    Game g = Game();
    g.display();
    
    std::cout << "enter thee coords\n";
    int i, j;
    scanf("%d %d", &i, &j);
    
    std::vector<char> c = g.getDiegonalLR(i, j);
    for (int k = 0; k < c.size(); k++) {
        std::cout << c[k] << "-";
    }
}
