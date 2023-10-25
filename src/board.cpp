#include <iostream>
#include "player.cpp"

class Coord {
public:
    int row;
    int col;

    std::string toString() const {
        std::string r = std::string(3, 0);
        std::sprintf(&(r[0]), "%d", row);
        r[1] = ':';
        std::sprintf(&(r[2]), "%d", col);
        return r;
    }

    bool operator==(const Coord& c) const {
        return (c.row == row) && (c.col == col);
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
        int flip = 0;
        int j = c.col - 1;
        for(int i = c.row - 1; i >= 0 && j >= 0; i--) {
            if (board[i][j] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[i][j] == empty_square_marker || i == 0 || j == 0) {
                j++;
                for (i++;!(Coord{i, j} == c); i++) {
                    board[i][j] = p2->piece;
                    j++;
                } 
                break;
            }

            flip += 1;
            board[i][j] = p->piece;
            j--;
        }
    
        flip = 0;
        j = c.col + 1;
        for(int i = c.row + 1; i < 8 && j < 8; i++) {
            if (board[i][j] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[i][j] == empty_square_marker || i == 7 || j == 7) {
                j--;
                i--;
                for (;!(Coord{i, j} == c); i--) {
                    board[i][j] = p2->piece;
                    j--;
                } 
                break;
            }

            flip += 1;
            board[i][j] = p->piece;
            j++;
        }
    }
    
    void playDiegonalRL(Coord c, Player * p, Player * p2) {
        int flip = 0;
        int j = c.col + 1;
        for(int i = c.row - 1; i >= 0 && j < 8; i--) {
            if (board[i][j] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[i][j] == empty_square_marker || i == 0 || j == 7) {
                j--;
                for (i++;!(Coord{i, j} == c); i++) {
                    board[i][j] = p2->piece;
                    j--;
                } 
                break;
            }

            flip += 1;
            board[i][j] = p->piece;
            j++;
        }

        flip = 0;
        j = c.col - 1;
        for(int i = c.row + 1; i < 8 && j >= 0; i++) {
            if (board[i][j] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[i][j] == empty_square_marker || i == 7 || j == 0) {
                j++;
                for (i--;!(Coord{i, j} == c); i--) {
                    board[i][j] = p2->piece;
                    j++;
                } 
                break;
            }

            flip += 1;
            board[i][j] = p->piece;
            j--;
        }
    }

    void playVertical(Coord c, Player * p, Player * p2) {
        int flip = 0;

        for (int i = c.row - 1; i >= 0; i--) {
            if (board[i][c.col] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }
            
            if(board[i][c.col] == empty_square_marker || i == 0) {
                for(i++; i < c.row; i ++) {
                    board[i][c.col] = p2->piece;
                }
                break;
            }

            flip += 1;
            board[i][c.col] = p->piece;
        }

        flip = 0;
        for (int i = c.row + 1; i < 8; i++) {
            if (board[i][c.col] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }
            
            if(board[i][c.col] == empty_square_marker || i == 7) {
                for(i--; i > c.row; i--) {
                    board[i][c.col] = p2->piece;
                }
                break;
            }

            flip += 1;
            board[i][c.col] = p->piece;
        }
    }

    void playHorizontal(Coord c, Player * p, Player * p2) {
        int flip = 0;
        for (int i = c.col - 1; i >= 0; i--) {
            if (board[c.row][i] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[c.row][i] == empty_square_marker || i == 0) {
                for(i++; i < c.col; i++) {
                    board[c.row][i] = p2->piece;
                }
                break;
            }

            flip += 1;
            board[c.row][i] = p->piece;
        }

        flip = 0;
        for (int i = c.col + 1; i < 8; i++) {
            if (board[c.row][i] == p->piece) {
                p->piece_count += flip;
                p2->piece_count -= flip;
                break;
            }

            if (board[c.row][i] == empty_square_marker || i == 7) {
                for(i--; i > c.col; i--) {
                    board[c.row][i] = p2->piece;
                }
                break;
            }

            flip += 1;
            board[c.row][i] = p->piece;
        }
    }
};
