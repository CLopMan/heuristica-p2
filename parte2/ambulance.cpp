#include "ambulance.hpp"

bool Ambulance::ev_pos(int x, int y) {
    if (0 <= x <= limit_x && 0 <= y <= limit_y) {
        return true;
    }
    return false;
}

void Ambulance::move_right() {
    if (ev_pos(position.x, position.y + 1)) {
        position.y += 1;
    }
}

void Ambulance::move_left() {
    if (ev_pos(position.x, position.y - 1)) {
        position.y -= 1;
    }
}

void Ambulance::move_up() {
    if (ev_pos(position.x - 1, position.y)) {
        position.x -= 1;
    }
}

void Ambulance::move_down() {
    if (ev_pos(position.x + 1, position.y)) {
        position.x += 1;
    }
}

void Ambulance::eval_slot(Casilla casilla) {
    energy -= casilla.get_cost();
    switch (casilla.get_type())
    {
    case contagioso:
        if (2 > cont_contagioso) cont_contagioso++;
        break;

    case no_contagioso:
        if (10 > cont_no_contagioso + cont_contagioso && (cont_contagioso == 0 || cont_no_contagioso < 8)) cont_no_contagioso++;
        break;
    
    default:
        break;
    }
}