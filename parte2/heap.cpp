#include "heap.hpp"

Heap::Heap(int n) {
    std::vector<Element> v;
    v.reserve(n);
    v.push_back(Element{State{}, 9'999'999});
    v.push_back(Element{State{}, -1});
    contents = v;
}

State Heap::pop() {
    
    State s = contents[contents.size() - 2].s;
    contents.erase(contents.end() - 2);
    return s;

}

bool Heap::is_empty() {
    return contents.size() - 2 == 0;
}

int Heap::insert(State s, int f) {
    int start = 0, end = contents.size() - 1;
    Element e{s, f};
    
    do {
    int index = (start + end) / 2;
    Element curr = contents[index];

    if (curr.f <= e.f) {
        end = index - 1;
    } else {
        start = index + 1;
    }
    if (start > end) start = end;
    if (end < start) end = start;

} while (start != end);
int offset = 0;
if (contents[start].f > e.f) offset = 1;
contents.insert(std::next(contents.begin(), start + offset), e);
for (auto e : contents) {
}
return start;
    
}