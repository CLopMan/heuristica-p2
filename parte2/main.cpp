
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

struct Cerrada{
    std::unordered_map<std::string, int> g{};
    std::unordered_map<std::string, int> f{};

    void insert(State current, int g, int f){
        this->g[current.to_string()] = g;
        this->f[current.to_string()] = f;
    }

};

int h1(State s, Map map){
    return 0;
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



void sucesors_new(State current, std::unordered_map<std::string, State> & previous, Cerrada &cerrada, Heap & frontier, Map & map){
    std::vector<State> neighbors;
    std::vector<int> costes_neigh;
    std::vector<State> states(9);
    std::vector<int> costes(9);
    // Genero los sucesores
    for (int i = 0; i < 9; i++){states[i] = current;}
    costes[0] = states[0].move_right(map);
    costes[1] = states[1].move_left(map);
    costes[2] = states[2].move_up(map);
    costes[3] = states[3].move_down(map);
    costes[4] = states[4].pick_up_contagioso();
    costes[5] = states[5].pick_up_ncontagioso();
    costes[6] = states[6].drop_contagioso(map);
    costes[7] = states[7].drop_ncontagioso(map);
    costes[8] = states[8].recharge(map);
    for (int i = 0; i < 9;i++){
        if (states[i]!= current){
            neighbors.push_back(states[i]);
            costes_neigh.push_back(costes[i]);
        }
    }
    
    for (int i = 0; i < neighbors.size();i++){
        State next = neighbors[i];
        int new_cost = cerrada.g[current.to_string()] + costes_neigh[i];
        int new_f = new_cost + h1(next,map);
        // Si no está en cerrada o está en cerrada pero con un coste mayor, lo meto en previous
        if ((cerrada.f.find(next.to_string()) != cerrada.f.end() && new_f < cerrada.f[next.to_string()]) || cerrada.f.find(next.to_string()) == cerrada.f.end()){
            previous[next.to_string()] = current;
        }
        // Lo meto en abierta
        frontier.insert(next, new_cost, new_f);
    }

    // Imprimo los sucesores
    for (int i = 0; i < neighbors.size();i++){
        std::cout << "neighbors: " << neighbors[i].to_string() << std::endl;
    }
}

std::vector<State> a_star_new(State origin, State final, std::function<int(State,Map)> h, Map map){
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
}

int main (int argc, char** argv) {
    Map map(argv[1]);
    map.print();
    State origin(map);
    State final(map);
    final.set_final(); 
    std::cout << "origin: " << origin.to_string() << std::endl;
    std::vector<State> path = a_star_new(origin, final, h1, map);


    return 0;
}