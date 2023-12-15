#include "map.hpp"
#include <fstream>
#include <iostream>

Map::Map(std::string path) {
    name = path;
    std::ifstream file(path, std::ios::in);
    std::vector<Casilla> v; 
    ySize = 1;
    xSize = 0;
    if (!file.is_open()) {
        std::cerr << "Unable to open " << path << "\n";
    } else {
        std::string line; 
        std::string slot_name;
        bool first_row = true;
        while (file >> line) { // mientras quede archivo
            xSize++;
            slot_name.clear();
            for (char c : line) {
                if (c == ';') {
                    v.push_back(create_slot(slot_name));
                    slot_name.clear();
                    if (first_row) ySize++;
                } else slot_name.append(std::string(1, c));
            }
            v.push_back(create_slot(slot_name)); // último de la línea
            first_row = false;
        }
    }
    map = v;
    park = search_slot(parking);
    cc = search_slot(hospital_c);
    cn = search_slot(hospital_nc);
    
}

Casilla Map::create_slot(std::string name) {
    if (name == "N") return (Casilla(no_contagioso));
    if (name == "C") return (Casilla(contagioso));
    if (name == "CC") return (Casilla(hospital_c));
    if (name == "CN") return (Casilla(hospital_nc));
    if (name == "P") return (Casilla(parking));
    if (name == "X") return (Casilla(no_transitable));
    return (Casilla(normal, stoi(name)));
}

Position Map::search_slot(tipo_casilla tipo) {
    int x=0, y=0, aux = 0;
    for (Casilla c : map) {
        if (c.get_type() == tipo) {
            break;
        }
        y = (y + 1) % ySize;
        if (!(++aux % ySize)) x++;
    }
    return Position{x, y};
}

int Map::get_ill(tipo_casilla tipo) {
    int out = 0;
    for (Casilla c : map) {
        if (c.get_type() == tipo) out++;
    }
    return out;
}
std::vector<Position> Map::get_ill_pos(tipo_casilla tipo) {
    std::vector<Position> out;
    int x=0, y=0, aux = 0;
    for (Casilla c : map) {
        if (c.get_type() == tipo) {
            out.push_back(Position{x, y});
        }
        y = (y + 1) % ySize;
        if (!(++aux % ySize)) x++;
    }
    return out;
}


void Map::print() {
    std::cout << xSize << "x" << ySize << "\n";
    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            switch (this->get_slot(i, j).get_type())
            {
            case contagioso:
                std::cout << "C;";
                break;
            case no_contagioso:
                std::cout << "N;";
                break;
            case parking:
                std::cout << "P;";
                break;
            case hospital_c:
                std::cout << "CC;";
                break;
            case hospital_nc:
                std::cout << "CN;";
                break;
            case no_transitable:
                std::cout << "X;";
                break;
            case normal:
                std::cout << this->get_slot(i, j).get_cost() << ";";
                break;
            
            default:
                break;
            }
        }
        std::cout << "\n";

    }
}