#ifndef POSITION_HPP
#define POSITION_HPP
struct Position {
    int  x;
    int y;

    bool operator==(const Position & other) const {
        return (x == other.x && y==other.y);
    }
};

#endif