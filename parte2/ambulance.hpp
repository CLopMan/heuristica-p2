#ifndef AMBULANCE_HPP

#define AMBULANCE_HPP

#include <iostream>
#include <array>
#include "map.hpp"

int constexpr ENERGIA_INICIAL = 50;
struct Position {
    int  x;
    int y;
};


class Ambulance {
private:
    Position position;
    int cont_contagioso; // los que van dentro
    int cont_no_contagioso; // los que van dentro
    int energy;
    int limit_x;
    int limit_y;
    bool ev_pos(int x, int y);

public: 
    Ambulance(int xParking, int yParking) : position{xParking, yParking}, cont_contagioso(0), cont_no_contagioso(0), energy(ENERGIA_INICIAL) {}
    Position get_position() {return position;};
    void move_right(); 
    void move_left();
    void move_up();
    void move_down();
    void eval_slot(Casilla casilla);
    void pick_up_contagioso();
    void pick_up_ncontagioso();
    void drop_contagioso();
    void drop_ncontagioso();
};

#endif