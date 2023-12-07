#ifndef STATE_HPP

#define STATE_HPP

#include "ambulance.hpp"
#include "position.hpp"
#include "map.hpp"

struct State {
    // atributtes
    Ambulance ambulance;
    int contagiosos; 
    int no_contagiosos; 

    // constructor
    State(Map map) {
        Position pos_parking = map.search_slot(parking);
        ambulance = Ambulance(pos_parking.x, pos_parking.y, map.get_ySize() - 1, map.get_xSize()- 1);
        contagiosos = map.get_ill(contagioso);
        no_contagiosos = map.get_ill(no_contagioso);

    };
    State() = default;
    //void final_gen(Map map);

    // operands
    void move_right() {ambulance.move_right();}; 
    void move_left() {ambulance.move_left();};
    void move_up() {ambulance.move_up();};
    void move_down() {ambulance.move_down();};
    
    void pick_up_contagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == contagioso) ambulance.pick_up_contagioso();
    };
    void pick_up_ncontagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == no_contagioso) ambulance.pick_up_ncontagioso();
    };
    void drop_contagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == hospital_c) ambulance.drop_contagioso();
    };
    void drop_ncontagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == hospital_nc) ambulance.drop_ncontagioso();
    };

    // neighbors
    std::vector<State> neighbors(Map map){
        std::vector<State> neighbors;
        std::vector<State> states(8);
        for (int i = 0; i < 8;i++){
            states[i] = *this;
        }
        states[0].move_right();
        states[1].move_left();
        states[2].move_up();
        states[3].move_down();
        states[4].pick_up_contagioso(map);
        states[5].pick_up_ncontagioso(map);
        states[6].drop_contagioso(map);
        states[7].drop_ncontagioso(map);
        for (int i = 0; i < 8;i++){
            if (states[i] != *this){
                neighbors.push_back(states[i]);
            }
        }
        return neighbors;
    };
    // state to key
    std::string to_string() {
        std::string key = std::to_string(ambulance.get_position().x)+ "/" + std::to_string(ambulance.get_position().y) + "/" + std::to_string(ambulance.cont_contagioso) + "/" + std::to_string(ambulance.cont_no_contagioso) + "/" + std::to_string(ambulance.energy) + "/" + std::to_string(contagiosos) + "/" + std::to_string(no_contagiosos);
        return key;
    };

    // operators
    bool operator==(State & other) {
        return {ambulance.get_position().x == other.ambulance.get_position().x && ambulance.get_position().y == other.ambulance.get_position().y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos};
    };
    bool operator!=(State & other) {
        return {!(ambulance.get_position().x == other.ambulance.get_position().x && ambulance.get_position().y == other.ambulance.get_position().y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos)};
    
    };

    void set_final() {
        contagiosos = 0; 
        no_contagiosos = 0;
    };
};

#endif