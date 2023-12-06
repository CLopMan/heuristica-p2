#include "ambulance.hpp"
#include "map.hpp"

struct State {
    // atributtes
    Ambulance ambulance;
    int contagiosos; 
    int no_contagiosos; 

    // constructor
    void origin_gen(Map map);
    void final_gen(Map map);

    // operands
    void move_right() {ambulance.move_right();}; 
    void move_left() {ambulance.move_left();};
    void move_up() {ambulance.move_up();};
    void move_down() {ambulance.move_down();};
    void pick_up_contagioso();
    void pick_up_ncontagioso();
    void drop_contagioso();
    void drop_ncontagioso();

};
