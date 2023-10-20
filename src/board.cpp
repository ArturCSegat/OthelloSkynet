#include <cstdio>
#include <vector>
#include <iostream>
#include "player.cpp"

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
    
    bool operator==(Coord const& c) {
        if (c.row == row && c.col == col) {
            return true;
        }
        return false;
    }
};

class Board {
public:
    char board[8][8];
    char empty_square_marker;

    Board(char esm) {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = esm;
                count++;
            }
        }

        empty_square_marker = esm;
    }
    
    char& operator[] (Coord c) {
        return board[c.row][c.col];
    }

    void display() {
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
    
    void playDiegonalLR(Coord c, Player * p, Player * p2) {
        int j = c.col - 1;
        for(int i = c.row - 1; i >= 0 && j >= 0; i--) {
            if (board[i][j] == p->piece) {
                break;
            }

            if (board[i][j] == empty_square_marker) {
                j++;
                for (i++;!(Coord{i, j} == c); i++) {
                    std::cout << i << " " << j << "\n";
                    board[i][j] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                    j++;
                } 
                break;
            }

            p->piece_count += 1;
            p2->piece_count--;
            board[i][j] = p->piece;
            j--;
        }

        j = c.col + 1;
        for(int i = c.row + 1; i < 8 && j < 8; i++) {
            if (board[i][j] == p->piece) {
                break;
            }

            if (board[i][j] == empty_square_marker) {
                j--;
                i--;
                for (;!(Coord{i, j} == c); i--) {
                    board[i][j] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                    j--;
                } 
                break;
            }

            p->piece_count += 1;
            board[i][j] = p->piece;
            j++;
        }
    }
    
    void playDiegonalRL(Coord c, Player * p, Player * p2) {
        int j = c.col + 1;
        for(int i = c.row - 1; i >= 0 && j < 8; i--) {
            if (board[i][j] == p->piece) {
                break;
            }

            if (board[i][j] == empty_square_marker) {
                j--;
                for (i++;!(Coord{i, j} == c); i++) {
                    board[i][j] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                    j--;
                } 
                break;
            }

            p->piece_count += 1;
            board[i][j] = p->piece;
            j++;
        }

        j = c.col - 1;
        for(int i = c.row + 1; i < 8 && j >= 0; i++) {
            if (board[i][j] == p->piece) {
                break;
            }

            if (board[i][j] == empty_square_marker) {
                j++;
                for (i--;!(Coord{i, j} == c); i--) {
                    board[i][j] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                    j++;
                } 
                break;
            }

            p->piece_count += 1;
            board[i][j] = p->piece;
            j--;
        }
    }

    void playVertical(Coord c, Player * p, Player * p2) {
        for (int i = c.row - 1; i >= 0; i--) {
            if (board[i][c.col] == p->piece) {
                break;
            }
            
            if(board[i][c.col] == empty_square_marker) {
                for(i++; i < c.row; i ++) {
                    board[i][c.col] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                }
                break;
            }

            p->piece_count += 1;
            board[i][c.col] = p->piece;
        }

        for (int i = c.row + 1; i < 8; i++) {
            if (board[i][c.col] == p->piece) {
                break;
            }
            
            if(board[i][c.col] == empty_square_marker) {
                for(i--; i > c.row; i--) {
                    board[i][c.col] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                }
                break;
            }

            board[i][c.col] = p->piece;
            p->piece_count += 1;
        }
    }

    void playHorizontal(Coord c, Player * p, Player * p2) {
        for (int i = c.col - 1; i >= 0; i--) {
            if (board[c.row][i] == p->piece) {
                break;
            }

            if (board[c.row][i] == empty_square_marker) {
                for(i++; i < c.col; i++) {
                    board[c.row][i] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                }
                break;
            }

            board[c.row][i] = p->piece;
            p->piece_count += 1;
        }

        for (int i = c.col + 1; i < 8; i++) {
            if (board[c.row][i] == p->piece) {
                break;
            }

            if (board[c.row][i] == empty_square_marker) {
                for(i--; i > c.col; i--) {
                    board[c.row][i] = p2->piece;
                    p2->piece_count++;
                    p->piece_count--;
                }
                break;
            }

            board[c.row][i] = p->piece;
            p->piece_count += 1;
        }
    }
};
