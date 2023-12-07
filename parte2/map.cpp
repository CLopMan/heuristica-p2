#include "map.hpp"
#include <fstream>
#include <iostream>

Map::Map(std::string path) {
    std::ifstream file(path, std::ios::in);
    std::vector<Casilla> v; 
    ySize = 0;
    xSize = 0;
    if (!file.is_open()) {
        std::cerr << "Unable to open " << path << "\n";
    } else {
        std::string line; 
        std::string slot_name;
        bool first_row = true;
        while (file >> line) { // while is archive left
            xSize++;
            slot_name.clear();
            for (char c : line) {
                if (c == ';') {
                    v.push_back(create_slot(slot_name));
                    slot_name.clear();
                    if (first_row) ySize++;
                } else slot_name.append(std::string(1, c));
            }
            first_row = false;
        }
    }
    map = v;
    
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
        y = (y + 1) % xSize; // 3
        if (!(++aux % xSize)) x++;
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


void Map::print() {
    std::cout << xSize << "x" << ySize << "\n";
    /*for (Casilla c : map) {
        switch (c.get_type())
            {
            case contagioso:
                std::cout << "C;";
                break;
            case no_contagioso:
                std::cout << "NC;";
                break;
            case parking:
                std::cout << "P;";
                break;
            case hospital_c:
                std::cout << "CC;";
                break;
            case hospital_nc:
                std::cout << "NC;";
                break;
            case no_transitable:
                std::cout << "X;";
                break;
            case normal:
                std::cout << c.get_cost() << ";";
                break;
            }
    }*/
    for (int i = 0; i < xSize; i++) {
        for (int j = 0; j < ySize; j++) {
            switch (this->get_slot(i, j).get_type())
            {
            case contagioso:
                std::cout << "C;";
                break;
            case no_contagioso:
                std::cout << "NC;";
                break;
            case parking:
                std::cout << "P;";
                break;
            case hospital_c:
                std::cout << "CC;";
                break;
            case hospital_nc:
                std::cout << "NC;";
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



/*
3x4

0, 0 ; 0, 1; 0, 2; 
1, 0 ; 1, 1; 1, 2; 
2, 0 ; 2, 1; 2, 2; 
3, 0 ; 3, 1; 3, 2; 




*/