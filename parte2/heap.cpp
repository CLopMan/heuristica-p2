#include "heap.hpp"

Heap::Heap(int n) {
    std::vector<Element> v;
    v.reserve(n);
    v.push_back(Element{State{}, 999'999});
    v.push_back(Element{State{}, -1});
    contents = v;
}

int Heap::pop() {
    
    //State s = contents[contents.size() - 2].s;
    Element e = contents[contents.size() - 2];
    contents.erase(contents.end() - 2);
    return e.f;

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