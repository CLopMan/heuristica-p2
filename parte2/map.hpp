#include <vector>
#include <string>

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