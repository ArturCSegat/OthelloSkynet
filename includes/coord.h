#ifndef COORD_H
#define COORD_H
#include <string>
class Coord {
public:
    int row;
    int col;

    std::string toString() const;
    bool operator==(const Coord&) const;
};
#endif
