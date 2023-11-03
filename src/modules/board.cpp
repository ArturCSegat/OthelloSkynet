#include <memory>
#include <iostream>
#include <vector>
#include "../../includes/player.h"
#include "../../includes/board.h"

Board::Board(char esm) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = esm;
            count++;
        }
    }

    empty_square_marker = esm;
}
    
char& Board::operator[] (Coord c) {
    return board[c.row][c.col];
}

void Board::display() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!j) {
                std::cout << i << " |";
            }

            std::cout << board[i][j] << "|";
        }
        std::cout << "\n";
    }

    std::cout << " ";
    for(int i = 0; i < 8; i++) {
        std::cout << " " << i;
    }
    std::cout << "\n";
}
    
void Board::playDiegonalLR(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill) {
    int j = c.col - 1;
    for(int i = c.row - 1; i >= 0 && j >= 0; i--) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == 0 || j == 0) {
            j++;
            for (i++;!(Coord{i, j} == c); i++) {
                // board[i][j] = p2->piece;
                fill.pop_back();
                j++;
            } 
            break;
        }

        // board[i][j] = p->piece;
        fill.push_back(Coord{i, j});
        j--;
    }

    j = c.col + 1;
    for(int i = c.row + 1; i < 8 && j < 8; i++) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == 7 || j == 7) {
            j--;
            i--;
            for (;!(Coord{i, j} == c); i--) {
                // board[i][j] = p2->piece;
                fill.pop_back();
                j--;
            } 
            break;
        }

        // board[i][j] = p->piece;
        fill.push_back(Coord{i, j});
        j++;
    }
}
    
void Board::playDiegonalRL(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill) {
    int j = c.col + 1;
    for(int i = c.row - 1; i >= 0 && j < 8; i--) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == 0 || j == 7) {
            j--;
            for (i++;!(Coord{i, j} == c); i++) {
                fill.pop_back();
                j--;
            } 
            break;
        }

        fill.push_back(Coord{i, j});
        j++;
    }

    j = c.col - 1;
    for(int i = c.row + 1; i < 8 && j >= 0; i++) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == 7 || j == 0) {
            j++;
            for (i--;!(Coord{i, j} == c); i--) {
                fill.pop_back();
                j++;
            } 
            break;
        }

        fill.push_back(Coord{i, j});
        j--;
    }
}

void Board::playVertical(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill) {
    for (int i = c.row - 1; i >= 0; i--) {
        if (board[i][c.col] == p->piece) {
            break;
        }

        if(board[i][c.col] == empty_square_marker || i == 0) {
            for(i++; i < c.row; i ++) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{i, c.col});
    }

    for (int i = c.row + 1; i < 8; i++) {
        if (board[i][c.col] == p->piece) {
            break;
        }

        if(board[i][c.col] == empty_square_marker || i == 7) {
            for(i--; i > c.row; i--) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{i, c.col});
    }
}

void Board::playHorizontal(Coord c, std::unique_ptr<Player>& p, std::unique_ptr<Player>& p2, std::vector<Coord>& fill) {
    for (int i = c.col - 1; i >= 0; i--) {
        if (board[c.row][i] == p->piece) {
            break;
        }

        if (board[c.row][i] == empty_square_marker || i == 0) {
            for(i++; i < c.col; i++) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{c.row, i});
    }

    for (int i = c.col + 1; i < 8; i++) {
        if (board[c.row][i] == p->piece) {
            break;
        }

        if (board[c.row][i] == empty_square_marker || i == 7) {
            for(i--; i > c.col; i--) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{c.row, i});
    }
}
