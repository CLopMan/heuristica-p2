
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

std::vector<Position> a_star(State origin, State goal, std::function<int(State)> h);

int main (int argc, char** argv) {
    Map const map(argv[1]);
    State origin;
    origin.origin_gen(map);
    State final;
    origin.final_gen(map);


    return 0;
}