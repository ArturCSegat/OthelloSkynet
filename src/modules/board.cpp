#include <memory>
#include <iostream>
#include <vector>
#include "../../includes/player.h"
#include "../../includes/board.h"
#include "../../includes/game.h"


Board::Board() {
    int count = 0;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            board[i][j] = ' ';
            count++;
        }
    }

    empty_square_marker = ' ';
}

Board::Board(char esm) {
    int count = 0;
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            board[i][j] = esm;
            count++;
        }
    }

    empty_square_marker = esm;
}
    
char& Board::operator[] (Coord c) {
    return board[c.row][c.col];
}

char const& Board::operator[] (Coord c) const {
    return board[c.row][c.col];
}

void Board::display() {
    for (int i = 0; i < GAME_N; i++) {
        for (int j = 0; j < GAME_N; j++) {
            if (!j) {
                std::cout << i << " |";
            }

            std::cout << board[i][j] << "|";
        }
        std::cout << "\n";
    }

    std::cout << " ";
    for(int i = 0; i < GAME_N; i++) {
        std::cout << " " << i;
    }
    std::cout << "\n";
}
    
void Board::playDiegonalLR(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const {
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
    for(int i = c.row + 1; i < GAME_N && j < GAME_N; i++) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == GAME_N - 1 || j == GAME_N - 1) {
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
    
void Board::playDiegonalRL(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const {
    int j = c.col + 1;
    for(int i = c.row - 1; i >= 0 && j < GAME_N; i--) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == 0 || j == GAME_N - 1) {
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
    for(int i = c.row + 1; i < GAME_N && j >= 0; i++) {
        if (board[i][j] == p->piece) {
            break;
        }

        if (board[i][j] == empty_square_marker || i == GAME_N - 1 || j == 0) {
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

void Board::playVertical(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const {
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

    for (int i = c.row + 1; i < GAME_N; i++) {
        if (board[i][c.col] == p->piece) {
            break;
        }

        if(board[i][c.col] == empty_square_marker || i == GAME_N - 1) {
            for(i--; i > c.row; i--) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{i, c.col});
    }
}

void Board::playHorizontal(Coord c, const std::unique_ptr<Player>& p, const std::unique_ptr<Player>& p2, std::vector<Coord>& fill) const {
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

    for (int i = c.col + 1; i < GAME_N; i++) {
        if (board[c.row][i] == p->piece) {
            break;
        }

        if (board[c.row][i] == empty_square_marker || i == GAME_N - 1) {
            for(i--; i > c.col; i--) {
                fill.pop_back();
            }
            break;
        }

        fill.push_back(Coord{c.row, i});
    }
}
