#include "heap.hpp"

Heap::Heap(int n) {
    std::vector<Element> v;
    v.reserve(n);
    v.push_back(Element{State{}, 0 , __INT_MAX__});
    v.push_back(Element{State{}, 0 ,-1});
    contents = v;
}

Element Heap::pop() {
    if (this->is_empty()) {
        State empty;
        Element elem = {empty,-1,-1};
        return elem;
    }

    Element s = contents[contents.size() - 2];
    contents.erase(contents.end() - 2);
    return s;

}

bool Heap::is_empty() {
    return contents.size() - 2 == 0;
}

int Heap::insert(State s, int g, int f) {
    int start = 0, end = contents.size() - 1;
    Element e{s, g, f};
    do {
        int index = (start + end) / 2;
        Element curr = contents[index];
        if (curr.f <= e.f)
        {
            end = index - 1;
        }
        else
        {
            start = index + 1;
        }
        if (start > end)
            start = end;
        if (end < start)
            end = start;
    } while (start != end);

    int offset = 0;
    if (contents[start].f > e.f) offset = 1;
    contents.insert(std::next(contents.begin(), start + offset), e);
    return start;
    
}

int Heap::borrar(State s) {
    for (int i = 1; i < contents.size() - 1; ++i) {
        auto e = contents[i];
        if (e.s == s) {
            contents.erase(contents.begin() + i);
            std::cout << "borrado\n";
        }
    }
    return 0;
}