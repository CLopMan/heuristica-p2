
#include <iostream> 
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "state.hpp"
#include "map.hpp"
#include "ambulance.hpp"



int h1(State s);

std::vector<State> a_star(State origin, State goal, std::function<int(State,Map) > h, Map map){
    std::vector<State> path;
    std::unordered_map<State, State> previous{};
    std::unordered_map<State, int> g{};
    std::unordered_map<State, int> f{};
    Heap frontier{};
    frontier.push(origin);
    previous[origin] = origin;
    g[origin] = 0;
    f[origin] = h(origin,map) + g[origin];
    bool found = false;
    while (!frontier.empty() and !found){
        State current = frontier.pop();
        if (current == goal){
            found = true;
        }
        else{
            std::vector<State> neighbors = current.neighbors();
            for (State next : neighbors){
                int new_cost = g[current] + map.get_slot(next.ambulance.get_position().x,next.ambulance.get_position().y).get_cost();
                int new_f = new_cost + h(next,map);
                if ((previous.contains(next) && new_f >= f[next]) || !previous.contains(next)){
                    previous[next] = current;
                    g[next] = new_cost;
                    f[next] = new_f;
                    frontier.push(next);
                }
            }
        }   
    }
    State current = goal;
    while (!(current == origin)){
        path.push_back(current);
        current = previous[current];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

int main (int argc, char** argv) {
    Map const map(argv[1]);
    State origin;
    origin.origin_gen(map);
    State final;
    origin.final_gen(map);


    return 0;
}