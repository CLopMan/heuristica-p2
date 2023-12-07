
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



int h1(State s);

/*std::vector<State> a_star(State origin, State goal, std::function<int(State,Map) > h, Map map){
    std::vector<State> path;
    std::unordered_map<std::string, State> previous{};
    std::unordered_map<std::string, int> g{};
    std::unordered_map<std::string, int> f{};
    Heap frontier{};
    previous[origin.to_string()] = origin;
    g[origin.to_string()] = 0;
    f[origin.to_string()] = h(origin,map) + g[origin];
    frontier.insert(origin, f[origin.to_string()]);
    bool found = false;
    while (!frontier.is_empty() and !found){
        State current = frontier.pop();
        if (current == goal){
            found = true;
        }
        else{
            std::vector<State> neighbors = current.neighbors();
            for (State next : neighbors){
                int new_cost = g[current.to_string()] + map.get_slot(next.ambulance.get_position().x,next.ambulance.get_position().y).get_cost();
                int new_f = new_cost + h(next,map);
                if ((previous.contains(next) && new_f < f[next.to_string()]) || !previous.contains(next)){
                    previous[next.to_string()] = current;
                    g[next.to_string()] = new_cost;
                    f[next.to_string()] = new_f;
                    frontier.insert(next, f[next.to_string()]);
                }
            }
        }   
    }
    State current = goal;
    while (!(current == origin)){
        path.push_back(current);
        current = previous[current.to_string()];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
*/
int main (int argc, char** argv) {
    Map const map(argv[1]);
    State s1 (map);
    State s2 (map);
    State s3 (map);
    State s4 (map);
    State s5 (map);
    State s6 (map);

    Heap h(6);

    h.insert(s5, 8);
  
    h.insert(s1, 1);
 
    h.insert(s6, 10);
    
    h.insert(s4, 6);
   
    h.insert(s3, 4);
 
    h.insert(s2, 2);

    

    for (auto e : h.contents) {
        std::cout << e.f << ", ";
    }
    std::cout << "\n";
    while (!h.is_empty()) {
        std::cout << h.pop() << "\n";

        for (auto e : h.contents) {
            std::cout << e.f << ", ";
        }
        std::cout << "\n==============\n";
    }
    


    return 0;
}