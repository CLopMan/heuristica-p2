#ifndef MAP_HPP

#define MAP_HPP

#include <vector>
#include <string>
#include "position.hpp"

// costes de cada tipo de casilla
struct Costes {
    int contagioso = 1;
    int no_contagioso = 1;
    int parking = 1;
    int hospital_c = 1;
    int hospital_nc = 1;
    int no_transitable = 999'999;
};

// tipos de casilla posibles
enum tipo_casilla{
    contagioso,
    no_contagioso,
    parking, 
    hospital_c, 
    hospital_nc,
    normal,
    no_transitable
};

// Objeto casilla 
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
        default: // tipo normal
            coste = cost;
            break;
        }
    }

    tipo_casilla get_type() {return tipo;}; // getter del tipo
    int get_cost() {return coste;}; // getter del coste

    // devuelve el tipo de casilla como un str
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

// objeto del mapa
class Map {
private: 
    std::vector<Casilla> map;
    int xSize; // número de filas
    int ySize; // número de columnas
    

public:
    std::string name;
    Position park;
    Position cc;
    Position cn;
    Map(std::string path); // constructor 
    int get_xSize() {return xSize;}; 
    
    int get_ySize() {return ySize;}; 
    
    Position search_slot(tipo_casilla tipo);
    
    int get_ill(tipo_casilla tipo); // devuelve el número de casillas del tipo especificado. Se usa para obtener el número de enfermos
    
    std::vector<Position> get_ill_pos(tipo_casilla tipo); // devuelve una lista con las posiciones de los enfermos especificados
    
    Casilla get_slot(int x, int y) // devuelve una casilla dada una posición
    {
        int index = ySize*x + y;
        return map[index];
    }
    
    Casilla create_slot(std::string name); // crea una casilla en el mapa
    
    void print(); // imprime el mapa (usado para debug)
    
};

#endif