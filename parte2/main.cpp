
#include <iostream> 
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "state.hpp"
#include "map.hpp"
#include "ambulance.hpp"
#include "heap.hpp"

int constexpr inf = __INT_MAX__;
struct Cerrada{
    std::unordered_map<std::string, int> g{};
    std::unordered_map<std::string, int> f{};

    void insert(State current, int g, int f){
        this->g[current.to_string()] = g;
        this->f[current.to_string()] = f;
    }

};

struct maximo
{
    int max;
    Position pos;
};

int h1(State s, Map map){
    maximo max_cont{0, Position{0, 0}};
    maximo max_no_cont{0, Position{0, 0}};
    maximo max{0, Position{0, 0}};
    if (s.contagiosos != 0 || s.no_contagiosos != 0)
    {
        for(auto pos : s.no_contagiosos_pos){
            int dis = abs(pos.x - s.ambulance.position.x) + abs(pos.y  - s.ambulance.position.y);
            if(dis > max_no_cont.max){
                max_no_cont.max = dis;
                max_no_cont.pos = pos;
            }
        }
        for(auto pos : s.contagiosos_pos){
            int dis = abs(pos.x - s.ambulance.position.x) + abs(pos.y  - s.ambulance.position.y);
            if(dis > max_cont.max){
                max_cont.max = dis;
                max_cont.pos = pos;
            }
        }
        if(max_no_cont.max > max_cont.max){
            max = max_no_cont;
            Position hosp_nc = map.search_slot(hospital_nc);
            return max.max + abs(hosp_nc.x - max.pos.x) + abs(hosp_nc.y - max.pos.y);
        }
        else{
            max = max_cont;
            Position hosp_c = map.search_slot(hospital_c);
            return max.max + abs(hosp_c.x - max.pos.x) + abs(hosp_c.y - max.pos.y);
            
        }
    }
    else{
        Position park = map.search_slot(parking);
        return abs(s.ambulance.position.x - park.x) + abs(s.ambulance.position.y - park.y);
    }
}

int manhatan_distance(Position origin, Position end) {
    return abs(origin.x - end.x) + abs(origin.y - end.y);
}

maximo max_ill(State s, std::vector<Position> enfermo) {
    maximo max{0, {0,0}};
    for(auto pos : enfermo){
        int dis = manhatan_distance(pos, s.ambulance.position);
        if(dis > max.max){
            max.max = dis;
            max.pos = pos;
        }
    }
    return max;
    
}


int h2 (State s, Map map) {

    int out = 0; 
    if (s.ambulance.cont_contagioso == 1) {
        maximo max_cont = max_ill(s, s.contagiosos_pos);
        out += max_cont.max + manhatan_distance(max_cont.pos, map.cc);
    } else if (s.ambulance.cont_contagioso == 2) {
        out += manhatan_distance(s.ambulance.position, map.cc);
    } else {
        if (s.ambulance.cont_no_contagioso == 10) {
            out += manhatan_distance(s.ambulance.position, map.cn);
        }else if (s.ambulance.cont_no_contagioso == 9) {

        } else if (s.ambulance.cont_no_contagioso == 8) {

        } else {
            maximo max_nc = max_ill(s, s.no_contagiosos_pos);
            maximo max_cont = max_ill(s, s.contagiosos_pos);
            if (max_nc.max >= max_cont.max) {
                out += max_nc.max + manhatan_distance(max_nc.pos, map.cn);
            } else {
                out += max_cont.max + manhatan_distance(max_cont.pos, map.cc);
            }
            
            
        }
    }
    out += manhatan_distance(s.ambulance.position, map.park);

    
    return out;
    
}


std::vector<State> sucesors(State current, Map & map){
    std::vector<State> neighbors_aux(4, current);
    std::vector<State> neighbors;

    // Genero los sucesores
    std::vector<bool> aplicable (4, false);
    aplicable[0] = neighbors_aux[0].move_right(map);
    aplicable[1] = neighbors_aux[1].move_left(map);
    aplicable[2] = neighbors_aux[2].move_up(map);
    aplicable[3] = neighbors_aux[3].move_down(map);
    for (int i = 0; i < neighbors_aux.size();i++){
        if (aplicable[i]) neighbors.push_back(neighbors_aux[i]);
    }
    return neighbors;
}


std::vector<State> reconstruct_path(std::unordered_map<std::string, State> previo, Element goal) {
    State s = goal.s;
    std::cout << "coste: " << goal.g << "\n";
    std::vector<State> path;
    path.push_back(s);
    while (previo[s.to_string()] != s) {
        path.push_back(previo[s.to_string()]);
        s = previo[s.to_string()];
    }
    return path;
}


std::vector<State> a_star(State origin, State final, std::function<int(State,Map)> h, Map & map){
    Heap abierta(10'000); // nodos pendientes por explorar, ordenados por función de evaluación
    abierta.insert(origin, -1, 0 + h(origin, map)); 
    Cerrada cerrada{}; // lista de nodos explorados, se almacena la g
    cerrada.insert(origin, 0, h(origin, map));
    std::unordered_map<std::string, State> previo; 
    previo[origin.to_string()] = origin; // el origen tiene a sí mismo como antecesor

    Element current; 
    int i = 0;
    while (!abierta.is_empty()) { // si hay nodos en abierta => explorar 
      
        current = abierta.pop(); // extracción del nodo con mejor f
        if (!cerrada.g.contains(current.s.to_string())) {std::cout << "inicialidado a inf\n"; cerrada.g[current.s.to_string()] = inf;} // si el nodo no se ha explorado antes, su g temporal es infinito
        if (current.g <= cerrada.g[current.s.to_string()]) {  // en la lista abierta pueden haber duplicados de un nodo con distinta g, ordenados por f => si previamente hemos explorado un camino mejor, descartamos este nodo. 


            if (current.s.compare_final(final)) {
                
                std::cout << "encontrado\n";
                return reconstruct_path(previo, current);

            }

            std::vector<State> neighbors = sucesors(current.s, map);

            for (State s : neighbors) {
                int step_cost = map.get_slot(s.ambulance.position.x, s.ambulance.position.y).get_cost();
                int temp_g = cerrada.g[current.s.to_string()] + step_cost;
                if (!cerrada.g.contains(s.to_string())) { // si no se ha explorado antes ese nodo, su g es infinita 
                    cerrada.g[s.to_string()] = inf;
                }
                if (cerrada.g[s.to_string()] > temp_g) {
                    cerrada.insert(s, temp_g, temp_g + h(s, map)); // actualización de g si obtenemos una mejor g
                    previo[s.to_string()] = current.s;
                    abierta.insert(s, temp_g, temp_g + h(s, map)); // insertamos el nodo en abierta por ser un camino candidato 
                }
            }
        } else {
            std::cout << "descartado\n";
        }
        

    }

    return {};
}


std::vector<State> a_star_v2(State origin, State final, std::function<int(State,Map)> h, Map & map) {
    Heap abierta(10'000);
    std::unordered_map<std::string, State> previo; 
    Cerrada cerrada; 
    previo[origin.to_string()] = origin;
    int nodos = 0; 

    abierta.insert(origin, 0, h(origin, map));
    cerrada.insert(origin, 0, h(origin, map));


    while (!abierta.is_empty()) {
        Element current = abierta.pop(); // extracción del primer elemento de abierta
        nodos++;
        if (!(nodos % 1'000)) std::cout << "expandidos: " << nodos <<"\n";

        if (current.s.compare_final(final)) return reconstruct_path(previo, current);
        
        std::vector<State> S = sucesors(current.s, map); 

        for (State s : S) {
            if (!cerrada.g.contains(s.to_string())) { // si no se ha explorado antes ese nodo, su g es infinita 
                cerrada.g[s.to_string()] = inf;
            }
            int step_cost = map.get_slot(s.ambulance.position.x, s.ambulance.position.y).get_cost();
            int temp_g = cerrada.g[current.s.to_string()] + step_cost;
            
            if (temp_g < cerrada.g[s.to_string()]) {
                previo[s.to_string()] = current.s;
                cerrada.insert(s, temp_g, temp_g + h(s, map));
                abierta.insert(s, temp_g, temp_g + h(s, map));
            }

        }

    }



    return {};
}



void output_str(std::vector<State> path, Map & map) {
    for (int i = path.size() - 1; i >= 0; --i) {
        std::cout << "(" << path[i].ambulance.position.x << ", " << path[i].ambulance.position.y << "):" << map.get_slot(path[i].ambulance.position.x, path[i].ambulance.position.y).get_type() << ":" <<path[i].ambulance.energy << "\n";
    }
}


int main (int argc, char** argv) {
    Map map(argv[1]);
    map.print();
    State origin(map);
    State final(map);
    final.set_final(); 
   // std::cout << "origin: " << origin.to_string() << std::endl;
    std::vector<State> path = a_star_v2(origin, final, h2, map);

    int i = path.size();
    std::cout << "printing path (size =" << i << "): \n";
    for (auto s : path) {
        std::cout << i-- << " : " << s.to_string() << "\n";
    }

    output_str(path, map);


    return 0;
}