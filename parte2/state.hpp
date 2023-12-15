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
        ambulance = Ambulance(pos_parking.x, pos_parking.y, map.get_xSize() - 1, map.get_ySize()- 1);
        contagiosos = map.get_ill(contagioso); // contagiosos no entregados
        no_contagiosos = map.get_ill(no_contagioso); // no contagiosos no entregados
        contagiosos_pos = map.get_ill_pos(contagioso); // posición de los contagiosos no recogidos
        no_contagiosos_pos = map.get_ill_pos(no_contagioso); // posición de los no contagiosos no recogidos


    };
    State() = default;
    //void final_gen(Map map);
    
    bool consecuences(Casilla slot) {
        switch (slot.get_type())
            {
            case parking:
                ambulance.recharge();
                break;

            case hospital_c:
                contagiosos -= ambulance.cont_contagioso;
                ambulance.drop_contagioso();
                break;

            case hospital_nc:
                if (ambulance.cont_contagioso == 0) { // sólo puedes soltar no contagiosos si ya has soltado contagiosos
                    no_contagiosos -= ambulance.cont_no_contagioso;
                    ambulance.drop_ncontagioso();
                }
                break;
            
            case contagioso:
                if (ambulance.cont_contagioso < 2 && ambulance.cont_no_contagioso <= 8) {// para recoger a un contagioso tienes que haber entregado a todos lo no contagiosos o tener a los que quedan en la ambulancia
                    for (int i = 0; i < contagiosos_pos.size(); ++i) {
                        if (ambulance.position.x == contagiosos_pos[i].x && ambulance.position.y == contagiosos_pos[i].y) {// posición de un contagioso pendiente de recoger
                                contagiosos_pos.erase(contagiosos_pos.begin() + i);
                                ambulance.pick_up_contagioso();
                            
                        }
                    }
                }
                break;
            
            case no_contagioso:
                if((ambulance.cont_contagioso == 0 && ambulance.cont_no_contagioso < 10)) // la ambulancia tiene capacidad para recoger un no contagioso
                    for (int i = 0; i < no_contagiosos_pos.size(); ++i) {
                        if (ambulance.position.x == no_contagiosos_pos[i].x && ambulance.position.y == no_contagiosos_pos[i].y) { // posición de un enfermo que aún no se ha recogido
                            no_contagiosos_pos.erase(no_contagiosos_pos.begin() + i); 
                            ambulance.pick_up_ncontagioso();
                        }
                        
                    }
                break;
            
            default:
                break;
            }
            return true;
    }

    // operands
    bool move_right (Map & map) {
        Casilla slot = map.get_slot(ambulance.position.x, ambulance.position.y + 1);
        int coste_casilla = slot.get_cost();

        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y + 1) && ambulance.energy >= coste_casilla) { // el movimiento está en los límites
            ambulance.move_right(); // movimiento
            ambulance.energy = ambulance.energy - coste_casilla;
            // consecuencias
            return consecuences(slot);
            
        }
        return false;
    }

    bool move_left (Map & map) {
        Casilla slot = map.get_slot(ambulance.position.x, ambulance.position.y - 1);
        int coste_casilla = slot.get_cost();

        if (ambulance.ev_pos(ambulance.position.x, ambulance.position.y - 1) && ambulance.energy >= coste_casilla) { // el movimiento está en los límites
            ambulance.move_left(); // movimiento
            ambulance.energy = ambulance.energy - coste_casilla;
            // consecuencias
            return consecuences(slot);
            
        }
        return false;
    }

    bool move_up (Map & map) {
        Casilla slot = map.get_slot(ambulance.position.x - 1, ambulance.position.y);
        int coste_casilla = slot.get_cost();
        
        if (ambulance.ev_pos(ambulance.position.x - 1, ambulance.position.y) && ambulance.energy >= coste_casilla) { // el movimiento está en los límites
            ambulance.move_up(); // movimiento
            ambulance.energy = ambulance.energy - coste_casilla;
            // consecuencias
            return consecuences(slot);
            
        }
        return false;
    }

    bool move_down (Map & map) {
        Casilla slot = map.get_slot(ambulance.position.x + 1, ambulance.position.y);
        int coste_casilla = slot.get_cost();
        
        if (ambulance.ev_pos(ambulance.position.x + 1, ambulance.position.y) && ambulance.energy >= coste_casilla) { // el movimiento está en los límites
            ambulance.move_down(); // movimiento
            ambulance.energy = ambulance.energy - coste_casilla;

            // consecuencias
            return consecuences(slot);
            
        }
        return false;
    }

    // state to key
    std::string to_string() {
        std::string pos_cont ="[";
        std::string pos_ncont = "[";

        for (Position p : contagiosos_pos) {
            pos_cont += "(" + std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
        }
        for (Position p : no_contagiosos_pos) {
            pos_ncont += std::to_string(p.x) + ", " + std::to_string(p.y) + ")";
        }
        pos_cont += "]";
        pos_ncont += "]";


        std::string key = std::to_string(ambulance.position.x)+ "/" + std::to_string(ambulance.position.y) + "/" + std::to_string(ambulance.cont_contagioso) + "/" + std::to_string(ambulance.cont_no_contagioso) + "/" + std::to_string(ambulance.energy) + "/" + std::to_string(contagiosos) + "/" + std::to_string(no_contagiosos) + "/" + pos_cont + "/" + pos_ncont;
        return key;
    };

    // operators
    bool operator==(State & other) {
        // Compara todos los atributos de dos estados
        return {ambulance.position.x == other.ambulance.position.x && ambulance.position.y == other.ambulance.position.y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos
        && contagiosos_pos == other.contagiosos_pos && no_contagiosos_pos== other.no_contagiosos_pos};
    };
    bool operator!=(State & other) {
        // Compara todos los atributos de dos estados 
        return {!(ambulance.position.x == other.ambulance.position.x && ambulance.position.y == other.ambulance.position.y 
        && ambulance.cont_contagioso == other.ambulance.cont_contagioso && ambulance.cont_no_contagioso == other.ambulance.cont_no_contagioso 
        && ambulance.energy == other.ambulance.energy && contagiosos == other.contagiosos && no_contagiosos == other.no_contagiosos)};
    
    };

    void set_final() {
        // Define un estado final
        contagiosos = 0; 
        no_contagiosos = 0;
        contagiosos_pos = {};
        no_contagiosos_pos = {};
    };

    bool compare_final(State state2){
        // Compara todos los atributos de dos estados excepto la energía
        return {ambulance.position.x == state2.ambulance.position.x && ambulance.position.y == state2.ambulance.position.y 
        && ambulance.cont_contagioso == state2.ambulance.cont_contagioso && ambulance.cont_no_contagioso == state2.ambulance.cont_no_contagioso 
        && contagiosos == state2.contagiosos && no_contagiosos == state2.no_contagiosos
        && contagiosos_pos == state2.contagiosos_pos && no_contagiosos_pos== state2.no_contagiosos_pos};
    };

};


#endif
