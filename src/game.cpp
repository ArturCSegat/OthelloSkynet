#include <cstdio>
#include <vector>
#include <iostream>

class Coord {
public:
    int row;
    int col;

    std::string string() {
        std::string r = std::string(3, 0);
        std::sprintf(&(r[0]), "%d", row);
        r[1] = ':';
        std::sprintf(&(r[2]), "%d", col);
        return r;
    }
};

class Game{
public:
    char board[8][8];
    int white_count;
    int black_count;
    
    Game() {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                std::sprintf(&(board[i][j]), "%d", count);
                count++;
            }
        }
        white_count = 0; 
        black_count = 0; 
    }

    void display() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                std::cout << board[i][j] << "|";
            }
            std::cout << "\n";
        }
    }
    
    std::vector<Coord> getDiegonalLR(Coord c) {
        std::vector<Coord> v;

        for (; c.row >= 0 && c.col >= 0; c.row--) {
            c.col--;
        }

        for (; c.row < 8 && c.col < 8; c.row++) {
            v.push_back(Coord{c.row, c.col});
            c.col++;
        }

        return v;
    }
    
    std::vector<Coord> getDiegonalRL(Coord c) {
        std::vector<Coord> v;

        for (;c.row > 0 && c.col < 7;c.row--) {
            c.col++;
        }
        std::cout << "i: " << c.row << "\n";
        std::cout << "up_col: " << c.col << "\n";

        for (; c.row < 8 && c.col >= 0; c.row++) {
            v.push_back(Coord{c.row, c.col});
            c.col--;
        }

        return v;
    }

    std::vector<Coord> getCol(Coord c) {
        std::vector<Coord> v;
        for (int i = 0; i < 8; i++) {
            v.push_back(Coord{i, c.col});
        }
        return v;
    }

    std::vector<Coord> getRow(Coord c) {
        std::vector<Coord> v;
        for (int i = 0; i < 8; i++) {
            v.push_back(Coord{c.row, i});
        }
        return v;
    }

    void play(int player, Coord c) {

    }
};

