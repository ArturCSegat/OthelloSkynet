#include <cstdio>
#include <vector>
#include <iostream>

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
            // for (int j = 0; j < 8; j++) {
            //     std::cout << "-";
            // }
            // std::cout << "\n";
        }
    }
    
    std::vector<char> getDiegonalLR(int row, int col) {
        std::vector<char> v = {};

        int up_col = col - 1;
        int i = row - 1;
        for (; i > 0 && up_col > 0; i--) {
            up_col--;
        }

        for (; i < 8 && up_col < 8; i++) {
            v.push_back(board[i][up_col]);
            up_col++;
        }

        return v;
    }
    
    std::vector<char> getDiegonalRL(int row, int col) {
        std::vector<char> v = {};

        int up_col = col - 1;
        for (int i = row - 1; i > 0 && up_col < 8; i--) {
            v.push_back(board[i][up_col]);
            up_col++;
        }

        int down_col = col - 1;
        for (int i = row + 1; i < 8 && down_col > 0; i++) {
            v.push_back(board[i][up_col]);
            up_col--;
        }

        return v;
    }

    std::vector<char> getCol(int row, int col) {
        std::vector<char> v = {};
        for (int i = 0; i < 8; i++) {
            v.push_back(board[i][col]);
        }
        return v;
    }

    std::vector<char> getRow(int row, int col) {
        std::vector<char> v = {};
        for (int i = 0; i < 8; i++) {
            v.push_back(board[row][i]);
        }
        return v;
    }
};
