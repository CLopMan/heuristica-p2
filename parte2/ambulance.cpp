#include "ambulance.hpp"

int constexpr CAPACIDAD_CONTAGIOSOS = 2;
int constexpr CAPACIDAD_NCONTAGIOSOS = 8;
int constexpr CAPACIDAD_TOTAL = 10;

bool Ambulance::ev_pos(int x, int y) {
    //if ((0 <= x <= limit_x) && (0 <= y <= limit_y)) {
    if ((0 <= x && x <= limit_x) && (0 <= y && y <= limit_y)) {
        return true;
    }
    return false;
}

bool Ambulance::move_right() {
    if (ev_pos(position.x, position.y + 1)) {
        position.y += 1;
        return true;
    }
    return false;
}

bool Ambulance::move_left() {
    if (ev_pos(position.x, position.y - 1)) {
        position.y -= 1;
        return true;
    }
    return false;
}

bool Ambulance::move_up() {
    if (ev_pos(position.x - 1, position.y)) {
        position.x -= 1;
        return true;
    }
    return false;
}

bool Ambulance::move_down() {
    if (ev_pos(position.x + 1, position.y)) {
        position.x += 1;
        return true;
    }
    return false;
}

void Ambulance::pick_up_contagioso() {cont_contagioso++;}

void Ambulance::pick_up_ncontagioso() {cont_no_contagioso++;}

void Ambulance::drop_contagioso() {cont_contagioso = 0;}

void Ambulance::drop_ncontagioso() {cont_no_contagioso = 0;}

void Ambulance::recharge() {energy = ENERGIA_INICIAL;}

// esto se debería borrar
/*void Ambulance::eval_slot(Map map) {
    energy -= casilla.get_cost();
    switch (casilla.get_type())
    {
    case contagioso:
        if (2 > cont_contagioso) cont_contagioso++;
        break;

    case no_contagioso:
        if (CAPACIDAD_TOTAL > cont_no_contagioso + cont_contagioso && (cont_contagioso == 0 || cont_no_contagioso < CAPACIDAD_NCONTAGIOSOS)) cont_no_contagioso++;
        break;
    
    default:
        break;
    }
}*/