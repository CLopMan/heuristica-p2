#ifndef HEAP_HPP

#define HEAP_HPP

#include "state.hpp"
#include <vector>

struct Element {
    State s;
    int g;
    int f;
};

class Heap {
public:
    std::vector<Element> contents;
//public:
    Heap(int n);
    Element pop(); // returns and deletes first element
    int insert(State s, int g, int f); // emplaces the element in the corresponding slot. Returns the index in which is inserted
    bool is_empty(); // returns true if lenght > 0
    int borrar(State s); // looks for a node an deletes it
    void print() {
        for (Element e : contents) {
            std::cout << e.s.to_string() << ", ";
        }
        std::cout << "\n";
    }
};

#endif