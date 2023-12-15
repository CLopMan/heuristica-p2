#ifndef MAP_HPP

#define MAP_HPP

#include <vector>
#include <string>
#include "position.hpp"

struct Costes {
    int contagioso = 1;
    int no_contagioso = 1;
    int parking = 1;
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
        // Constructor de Casilla
        this->tipo = tipo;
        Costes costes;
        // Según el tipo se define el coste
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

    std::string get_type_str() {
        switch (tipo)
            {
            case contagioso:
                return "C";
                break;
            case no_contagioso:
                return "N";
                break;
            case parking:
                return "P";
                break;
            case hospital_c:
                return "CC";
                break;
            case hospital_nc:
                return "CN";
                break;
            case no_transitable:
                return "X";
                break;
            case normal:
                return std::to_string(coste);
                break;
            
            default:
                return "no_valid";
                break;
            }
    }
};

class Map {
private: 
    std::vector<Casilla> map;
    int xSize; // tamaño de fila (número de comlumnas)
    int ySize; // tamaño de columna (número de filas)
    

public:
    std::string name;
    Position park;
    Position cc;
    Position cn;
    Map(std::string path);
    int get_xSize() {return xSize;};
    int get_ySize() {return ySize;};
    Position search_slot(tipo_casilla tipo);
    int get_ill(tipo_casilla tipo);
    std::vector<Position> get_ill_pos(tipo_casilla tipo);
    Casilla get_slot(int x, int y)
    {
        int index = ySize*x + y;
        return map[index];
    }
    Casilla create_slot(std::string name);
    void print();
    
};

#endif