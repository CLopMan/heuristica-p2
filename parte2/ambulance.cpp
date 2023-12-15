#include "ambulance.hpp"

int constexpr CAPACIDAD_CONTAGIOSOS = 2;
int constexpr CAPACIDAD_NCONTAGIOSOS = 8;
int constexpr CAPACIDAD_TOTAL = 10;

bool Ambulance::ev_pos(int x, int y) {
    if ((0 <= x && x <= limit_x) && (0 <= y && y <= limit_y)) {
        return true;
    }
    return false;
}

void Ambulance::move_right() {
    position.y += 1;
}

void Ambulance::move_left() {
    position.y -= 1;
}

void Ambulance::move_up() {
        position.x -= 1;
}

void Ambulance::move_down() {
    position.x += 1;
}

void Ambulance::pick_up_contagioso() {cont_contagioso++;}

void Ambulance::pick_up_ncontagioso() {cont_no_contagioso++;}

void Ambulance::drop_contagioso() {cont_contagioso = 0;}

void Ambulance::drop_ncontagioso() {cont_no_contagioso = 0;}

void Ambulance::recharge() {energy = ENERGIA_INICIAL;}
