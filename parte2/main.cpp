
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
    maximo max_cont{0, Position(0, 0)};
    maximo max_no_cont{0, Position(0, 0)};
    maximo max{0, Position(0, 0)};
    if (s.contagiosos != 0 || s.no_contagiosos != 0)
    {
        for(auto pos : s.no_contagiosos_pos){
            int dis = abs(pos.x + pos.y - s.ambulance.position.x - s.ambulance.position.y);
            if(dis > max_no_cont.max){
                max_no_cont.max = dis;
                max_no_cont.pos = pos;
            }
        }
        for(auto pos : s.contagiosos_pos){
            int dis = abs(pos.x + pos.y - s.ambulance.position.x - s.ambulance.position.y);
            if(dis > max_cont.max){
                max_cont.max = dis;
                max_cont.pos = pos;
            }
        }
        if(max_no_cont.max > max_cont.max){
            max = max_no_cont;
            Position hosp_nc = map.search_slot(hospital_nc);
            return abs(max.max + hosp_nc.x + hosp_nc.y - max.pos.x - max.pos.y);
        }
        else{
            max = max_cont;
            Position hosp_c = map.search_slot(hospital_c);
            return abs(max.max + hosp_c.x + hosp_c.y - max.pos.x - max.pos.y);
        }
    }
    else{
        Position park = map.search_slot(parking);
        return abs(s.ambulance.position.x + s.ambulance.position.y - park.x - park.y);
    }
}

/*void sucesors(State current, std::unordered_map<std::string, State> & previous, std::unordered_map<std::string, int> & g, std::unordered_map<std::string, int> & f, Heap & frontier, Map & map){
    std::vector<State> neighbors;
    std::vector<int> costes_neigh;
    std::vector<State> states(8);
    std::vector<int> costes(8);
    for (int i = 0; i < 8; i++)
    {
        states[i] = current;
    }
    costes[0] = states[0].move_right(map);
    costes[1] = states[1].move_left(map);
    costes[2] = states[2].move_up(map);
    costes[3] = states[3].move_down(map);
    costes[4] = states[4].pick_up_contagioso();
    costes[5] = states[5].pick_up_ncontagioso();
    costes[6] = states[6].drop_contagioso(map);
    costes[7] = states[7].drop_ncontagioso(map);
    for (int i = 0; i < 8;i++){
        if (states[i]!= current){
            neighbors.push_back(states[i]);
            costes_neigh.push_back(costes[i]);
        }
    }
    for (int i = 0; i < neighbors.size();i++){
        State next = neighbors[i];
        int new_cost = g[current.to_string()] + costes_neigh[i];
        int new_f = new_cost + h1(next,map);
        if ((previous.find(next.to_string()) != previous.end() && new_f < f[next.to_string()]) || previous.find(next.to_string()) == previous.end()){
            previous[next.to_string()] = current;
            g[next.to_string()] = new_cost;
            f[next.to_string()] = new_f;
            if() // si no está en frontier
                frontier.insert(next, f[next.to_string()]);
        }
    }
    //for (int i = 0; i < neighbors.size();i++){
    //            std::cout << "neighbors: " << neighbors[i].s.to_string() + " /g =" + std::to_string(g[neighbors[i].s.to_string()]) + " /f =" + std::to_string(f[neighbors[i].s.to_string()]) << std::endl;
    //        }
}*/

/*std::vector<State> a_star(State origin, State goal, std::function<int(State,Map) > h, Map map){
    std::vector<State> path;
    std::unordered_map<std::string, State> previous{};
    std::unordered_map<std::string, int> g{};
    std::unordered_map<std::string, int> f{};
    Heap frontier(1000);
    previous[origin.to_string()] = origin;
    g[origin.to_string()] = 0;
    f[origin.to_string()] = h(origin,map) + g[origin.to_string()];
    frontier.insert(origin, f[origin.to_string()]);
    bool found = false;
    State current;
    while (!frontier.is_empty() and !found)
    {
        std::cout << "*********************************" << std::endl;
        current = frontier.pop();
        std::cout << "current: " << current.to_string() + " coste=" + std::to_string(g[current.to_string()]) << std::endl;
        for (int i = 0; i < frontier.contents.size();i++){
            std::cout << "frontier: " << frontier.contents[i].s.to_string() + " coste=" + std::to_string(frontier.contents[i].f) << std::endl;
        }   
        if (current == goal)
        {
            found = true;
        }
        else{
            sucesors(current, previous, g, f, frontier, map);
        }
    }
    std::cout << "Solución encontrada" << std::endl;
    while (!(current == origin)){
        path.push_back(current);
        current = previous[current.to_string()];
    }
    std::reverse(path.begin(), path.end());
    for (int i = 0; i < path.size();i++){
        std::cout << "path: " << path[i].to_string() << std::endl;
    }
    return path;
}*/



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

/*std::vector<State> a_star_new(State origin, State final, std::function<int(State,Map)> h, Map map){
    std::vector<State> path;
    std::vector<int> costes;
    std::unordered_map<std::string, State> previous{};
    Cerrada cerrada{};
    Heap frontier(1000);
    bool found = false;
    Element elem_current;
    State current;
    frontier.insert(origin, 0, h(origin, map));
    previous[origin.to_string()] = origin;

    // Comienzo del algoritmo
    while (!frontier.is_empty() and !found){
        // Saco el primer elemento de abierta que no esté en cerrada
        bool en_cerrada = true;
        while (en_cerrada){
            elem_current = frontier.pop();            
            if (cerrada.g.find(elem_current.s.to_string()) == cerrada.g.end()){
                en_cerrada = false;
            }
        }
        current = elem_current.s;
        std::cout << "*********************************" << std::endl;
        std::cout << "current: " << current.to_string() + " coste=" + std::to_string(elem_current.g) << std::endl;
        // Si es el final, termino
        if (current == final){
            found = true;
        }
        // Si no, lo meto en cerrada y genero sus sucesores
        else{
            cerrada.insert(current,elem_current.g, elem_current.f);
            sucesors_new(current, previous, cerrada, frontier, map);
        }

        // Imprimo los contenidos de abierta, cerrada y previous
        for (int i = 0; i < frontier.contents.size();i++){
            std::cout << "frontier: " << frontier.contents[i].s.to_string() + " coste=" + std::to_string(frontier.contents[i].f) << std::endl;
        }
        for (auto e : cerrada.f) {
            std::cout << "cerrada: " << e.first << " " << e.second << std::endl;
        }
        for (auto e : previous) {
            std::cout << "previous: " << e.first << " " << e.second.to_string() << std::endl;
        }
    }
    // Si he encontrado solución, reconstruyo el camino
    if (found){
        
        std::cout << "Solución encontrada" << std::endl;
        while (!(current.to_string() == origin.to_string())){
            path.push_back(current);
            costes.push_back(cerrada.g[current.to_string()]);
            current = previous[current.to_string()];
        }
        path.push_back(origin);
        std::reverse(path.begin(), path.end());
        for (int i = 0; i < path.size();i++){
            std::cout << "path: " << path[i].to_string() << std::endl;
            std::cout << "coste: " << costes[i] << std::endl;
        }
        return path;
    }
    // Si no, devuelvo un vector vacío
    std::cout << "No se ha encontrado solución" << std::endl;
    return path;
}*/

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
        if (!(nodos % 1'000'000)) std::cout << "expandidos: " << nodos <<"\n";

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
    std::vector<State> path = a_star_v2(origin, final, h1, map);

    int i = path.size();
    std::cout << "printing path (size =" << i << "): \n";
    for (auto s : path) {
        std::cout << i-- << " : " << s.to_string() << "\n";
    }

    output_str(path, map);


    return 0;
}