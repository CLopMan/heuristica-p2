
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



int h1(State s, Map map){
    return 0;
}

std::vector<State> a_star(State origin, State goal, std::function<int(State,Map) > h, Map map){
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
    while (!frontier.is_empty() and !found){
        std::cout << "*********************************" << std::endl;
        State current = frontier.pop();
        std::cout << "current: " << current.to_string() + " coste=" + std::to_string(g[current.to_string()]) << std::endl;
        for (int i = 0; i < frontier.contents.size();i++){
            std::cout << "frontier: " << frontier.contents[i].s.to_string() + " coste=" + std::to_string(frontier.contents[i].f) << std::endl;
        }   
        if (current == goal)
        {
            found = true;
        }
        else{
            std::vector<State> neighbors = current.neighbors(map);
            for (State next : neighbors){
                int new_cost = g[current.to_string()] + map.get_slot(next.ambulance.position.x,next.ambulance.position.y).get_cost();
                int new_f = new_cost + h(next,map);
                if ((previous.find(next.to_string()) != previous.end() && new_f < f[next.to_string()]) || previous.find(next.to_string()) == previous.end()){
                    previous[next.to_string()] = current;
                    g[next.to_string()] = new_cost;
                    f[next.to_string()] = new_f;
                    frontier.insert(next, f[next.to_string()]);
                }
            }
            for (int i = 0; i < neighbors.size();i++){
                std::cout << "neighbors: " << neighbors[i].to_string() + " /g =" + std::to_string(g[neighbors[i].to_string()]) + " /f =" + std::to_string(f[neighbors[i].to_string()]) << std::endl;
            }
        }   
    }
    std::cout << "Solución encontrada" << std::endl;
    State current = goal;
    while (!(current == origin)){
        path.push_back(current);
        current = previous[current.to_string()];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main (int argc, char** argv) {
    std::cout << "Pasa por aqui" << std::endl;
    Map const map(argv[1]);
    std::cout << "Pasa por aqui" << std::endl;
    State origin(map);
    State final(map);
    final.set_final(); 
    std::cout << "origin: " << origin.to_string() << std::endl;
    std::vector<State> path = a_star(origin, final, h1, map);


    return 0;
}