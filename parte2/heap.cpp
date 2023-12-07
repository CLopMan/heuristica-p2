#include "heap.hpp"

Heap::Heap(int n) {
    std::vector<Element> v;
    v.reserve(n);
    contents = v;
    capacity = n;
}

State Heap::pop() {
    
    State s = contents[contents.size() - 1].s;
    contents.pop_back();
    return s;
    
    
}

bool Heap::is_empty() {
    return contents.size() == 0;
}

int Heap::insert(State s, int f) {
    if (contents.size() + 1 < capacity) {
        contents.resize(2*contents.size());
        capacity = 2*contents.size();
    }
    int start = 0, end = contents.size() - 1;
    Element e{s, f};
    if (this->is_empty()) {
        contents.push_back(e);
        return 0;}
    else {
        do {
        int index = (start + end) / 2;
        std::cout << "start: " << start << "\n" <<
                    "end: " << end << "\n" <<
                    "index: " << index << "\n";
        Element curr = contents[index];

        if (curr.f <= e.f) {
            end = index - 1;
        } else {
            start = index + 1;
        }
        if (start > end) start = end;
        if (end < start) end = start;
    } while (start != end);
    contents.insert(std::next(contents.begin(), start), e);
    return start;
    }
}