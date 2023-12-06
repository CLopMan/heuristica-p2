#include <iostream>
#include <array>
#include "map.hpp"
struct Position {
    int  x;
    int y;
};


class Ambulance {
private:
    Position position;
    int cont_contagioso;
    int cont_no_contagioso;
    int energy;
    int limit_x;
    int limit_y;
    bool ev_pos(int x, int y);

public: 
    Ambulance(int xParking, int yParking) : position{xParking, yParking}, cont_contagioso(0), cont_no_contagioso(0), energy(50) {}
    void move_right(); 
    void move_left();
    void move_up();
    void move_down();
    void eval_slot(Casilla casilla);
};