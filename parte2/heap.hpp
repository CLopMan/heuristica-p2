#ifndef HEAP_HPP

#define HEAP_HPP

#include "state.hpp"
#include <vector>
#include "element.hpp"
class Heap {
public:
    std::vector<Element> contents;
//public:
    Heap(int n);
    State pop(); // returns and deletes first element
    int insert(State s, int f); // emplaces the element in the corresponding slot. Returns the index in which is inserted
    bool is_empty(); // returns true if lenght > 0
};

#endif