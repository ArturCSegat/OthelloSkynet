#include <iostream>
#include "../../includes/coord.h"

std::string Coord::toString() const {
    std::string r = std::string(3, 0);
    std::sprintf(&(r[0]), "%d", row);
    r[1] = ':';
    std::sprintf(&(r[2]), "%d", col);
    return r;
}

bool Coord::operator==(const Coord& c) const {
    return (c.row == row) && (c.col == col);
}
