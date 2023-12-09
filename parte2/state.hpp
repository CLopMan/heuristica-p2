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
    std::vector <Position> contagiosos_pos;
    std::vector <Position> no_contagiosos_pos;
    // constructor
    State(Map map) {
        Position pos_parking = map.search_slot(parking);
        ambulance = Ambulance(pos_parking.x, pos_parking.y, map.get_ySize() - 1, map.get_xSize()- 1);
        contagiosos = map.get_ill(contagioso); // contagiosos no entregados
        no_contagiosos = map.get_ill(no_contagioso); // no contagiosos no entregados
        contagiosos_pos = map.get_ill_pos(contagioso); // posición de los contagiosos no recogidos
        no_contagiosos_pos = map.get_ill_pos(no_contagioso); // posición de los no contagiosos no recogidos


    };
    State() = default;
    //void final_gen(Map map);

    // operands
    void move_right() {ambulance.move_right();}; 
    void move_left() {ambulance.move_left();};
    void move_up() {ambulance.move_up();};
    void move_down() {ambulance.move_down();};
    
    /*void pick_up_contagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == contagioso) ambulance.pick_up_contagioso();
    };*/
    void pick_up_contagioso(Map map) {
        for (int i = 0; i < contagiosos_pos.size();i++){
            if (ambulance.position.x == contagiosos_pos[i].x && ambulance.position.y == contagiosos_pos[i].y) {
                contagiosos_pos.erase(contagiosos_pos.begin() + i);
                ambulance.pick_up_contagioso();
                break;
            }
        }
    };

    /*void pick_up_ncontagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == no_contagioso) ambulance.pick_up_ncontagioso();
    };*/
    void pick_up_ncontagioso(Map map) {
        for (int i = 0; i < no_contagiosos_pos.size();i++){
            if (ambulance.position.x == no_contagiosos_pos[i].x && ambulance.position.y == no_contagiosos_pos[i].y) {
                no_contagiosos_pos.erase(no_contagiosos_pos.begin() + i);
                ambulance.pick_up_ncontagioso();
                break;
            }
        }
    };
    void drop_contagioso(Map map) {
        if (map.get_slot(ambulance.position.x, ambulance.position.y).get_type() == hospital_c){
            ambulance.drop_contagioso();
            contagiosos--;
        } 
    };
    void drop_ncontagioso(Map map) {
        if (map.get_slot(ambulance.position.x, ambulance.position.y).get_type() == hospital_nc){
            ambulance.drop_ncontagioso();
            no_contagiosos--; 
        }
    };
    void recharge() {ambulance.recharge();};

    // neighbors
    std::vector<State> neighbors(Map map){
        std::vector<State> neighbors;
        std::vector<State> states(9);
        for (int i = 0; i < 9;i++){
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
        states[8].recharge();
        for (int i = 0; i < 9;i++){
            if (states[i] != *this){
                neighbors.push_back(states[i]);
            }
        }
        return neighbors;
    };
    // state to key
    std::string to_string() {
        std::string key = std::to_string(ambulance.position.x)+ "/" + std::to_string(ambulance.position.y) + "/" + std::to_string(ambulance.cont_contagioso) + "/" + std::to_string(ambulance.cont_no_contagioso) + "/" + std::to_string(ambulance.energy) + "/" + std::to_string(contagiosos) + "/" + std::to_string(no_contagiosos);
        return key;
    };

    // operators
    bool operator==(State & other) {
        return {ambulance.position.x == other.ambulance.position.x && ambulance.position.y == other.ambulance.position.y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos};
    };
    bool operator!=(State & other) {
        return {!(ambulance.position.x == other.ambulance.position.x && ambulance.position.y == other.ambulance.position.y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos)};
    
    };

    void set_final() {
        contagiosos = 0; 
        no_contagiosos = 0;
    };
};

#endif