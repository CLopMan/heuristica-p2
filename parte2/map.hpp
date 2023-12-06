#ifndef MAP_HPP

#define MAP_HPP

#include <vector>
#include <string>

struct Costes {
    int contagioso = 1;
    int no_contagioso = 1;
    int parking = -50;
    int hospital_c = 1;
    int hospital_nc = 1;
    int no_transitable = 999'999;
};


enum tipo_casilla{
    contagioso,
    no_contagioso,
    parking, 
    hospital_c, 
    hospital_nc,
    normal,
    no_transitable
};

class Casilla {
private:
    tipo_casilla tipo; 
    int coste; 
public:
    Casilla(tipo_casilla tipo, int cost = 0) {
        Costes costes;
        switch (tipo)
        {
        case contagioso:
            coste = costes.contagioso;
            break;
        case no_contagioso:
            coste = costes.no_contagioso;
            break;
        case parking:
            coste = costes.parking;
            break;
        case hospital_c:
            coste = costes.hospital_c;
            break;
        case hospital_nc:
            coste = costes.hospital_nc;
            break;
        case no_transitable:
            coste = costes.no_transitable;
            break;
        default: // normal
            coste = cost;
            break;
        }
    }
    tipo_casilla get_type() {return tipo;};
    int get_cost() {return coste;};
};

class Map {
private: 
    std::vector<std::vector<Casilla>> map;
    int xSize;
    int ySize;

public:
    Map(std::string path);
    int get_xSize() {return xSize;};
    int get_ySize() {return ySize;};
};

#endif