#ifndef STATE_HPP

#define STATE_HPP

#include "ambulance.hpp"
#include "position.hpp"
#include "map.hpp"

struct State {
    // atributtes
    Ambulance ambulance;
    int contagiosos; // contagiosos no entregados
    int no_contagiosos; // no contagioso no entregados
    std::vector <Position> contagiosos_pos; // posición de los contagiosos no recogidos
    std::vector <Position> no_contagiosos_pos; // posición de los no contagiosos no recogidos
    // constructor
    State(Map & map) {
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
    int move_right(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y + 1)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.y + 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_right();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
        };
    int move_left(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y - 1)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.y - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_left();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    int move_up(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x - 1, ambulance.position.y)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.x - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_up();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    int move_down(Map & map) {
        if(ambulance.ev_pos(ambulance.position.x + 1, ambulance.position.y)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.x - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_down();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    
    /*void pick_up_contagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == contagioso) ambulance.pick_up_contagioso();
    };*/
    int pick_up_contagioso() {
        for (int i = 0; i < contagiosos_pos.size();i++){
            if (ambulance.position.x == contagiosos_pos[i].x && ambulance.position.y == contagiosos_pos[i].y) {
                contagiosos_pos.erase(contagiosos_pos.begin() + i);
                ambulance.pick_up_contagioso();
            }
        }
        return 0;
    };

    /*void pick_up_ncontagioso(Map map) {
        if (map.get_slot(ambulance.get_position().x, ambulance.get_position().y).get_type() == no_contagioso) ambulance.pick_up_ncontagioso();
    };*/
    int pick_up_ncontagioso() {
        for (int i = 0; i < no_contagiosos_pos.size();i++){
            if (ambulance.position.x == no_contagiosos_pos[i].x && ambulance.position.y == no_contagiosos_pos[i].y) {
                no_contagiosos_pos.erase(no_contagiosos_pos.begin() + i);
                ambulance.pick_up_ncontagioso();
            }
        }
        return 0;
    };
    int drop_contagioso(Map & map) {
        if (map.get_slot(ambulance.position.x, ambulance.position.y).get_type() == hospital_c){
            int contagiosos_entregar = ambulance.cont_contagioso;
            ambulance.drop_contagioso();
            contagiosos -= contagiosos_entregar;
        }
        return 0;
    };
    int drop_ncontagioso(Map & map) {
        if (map.get_slot(ambulance.position.x, ambulance.position.y).get_type() == hospital_nc){
            int no_contagiosos_entregar = ambulance.cont_no_contagioso;
            ambulance.drop_ncontagioso();
            no_contagiosos -= no_contagiosos_entregar;
        }
        return 0;
    };
    int recharge(Map & map) {
        if (map.get_slot(ambulance.position.x, ambulance.position.y).get_type() == parking){
            ambulance.recharge();
        }
        return 0;
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

    static bool compare_final(State state1 , State state2){
        return {state1.ambulance.position.x == state2.ambulance.position.x && state1.ambulance.position.y == state2.ambulance.position.y 
        && state1.ambulance.cont_contagioso == state2.ambulance.cont_contagioso && state1.ambulance.cont_no_contagioso == state2.ambulance.cont_no_contagioso 
        && state1.contagiosos == state2.contagiosos && state1.no_contagiosos == state2.no_contagiosos};
    };





    int move_right_recharge(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y + 1)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.y + 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_right();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    int move_left_recharge(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y - 1)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.y - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_left();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    int move_up_recharge(Map & map) {
        if (ambulance.ev_pos(ambulance.position.x - 1, ambulance.position.y)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.x - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_up();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };
    int move_down_recharge(Map & map) {
        if(ambulance.ev_pos(ambulance.position.x + 1, ambulance.position.y)){
            int aux_energy = map.get_slot(ambulance.position.x, ambulance.position.x - 1).get_cost();
            int diff_energy = ambulance.energy - aux_energy;
            if (diff_energy >= 0){
                ambulance.move_down();
                ambulance.energy = diff_energy;
                return aux_energy;
            }
        }
        return 0;
    };

};


#endif
