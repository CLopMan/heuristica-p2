
#include <iostream> 
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <fstream>
#include <chrono>

#include "state.hpp"
#include "map.hpp"
#include "ambulance.hpp"
#include "heap.hpp"

int constexpr inf = __INT_MAX__;

int nodos = 0;
int coste = 0;
int longitud = 0;





struct Cerrada{
    // g es el coste del camino desde el nodo inicial hasta el nodo actual
    std::unordered_map<std::string, int> g{};
    // f es el coste estimado del camino desde el nodo actual hasta el nodo final
    std::unordered_map<std::string, int> f{};

    void insert(State current, int g, int f){
        this->g[current.to_string()] = g;
        this->f[current.to_string()] = f;
    }

};

struct maximo
{
    int max;
    Position pos;
};

int manhatan_distance(Position origin, Position end) {
    // Distancia de manhatan
    return abs(origin.x - end.x) + abs(origin.y - end.y);
}

std::vector<int> get_vector_min_dist(Position origin, std::vector<Position> enfermo){
    // Devuelve un vector con los indices de los enfermos ordenados siguiendo el mejor camino posible
    std::vector<int> indices;
    std::vector<bool> consultado (enfermo.size(), false);
    int indice_auxiliar=0;
    int minimo_aux = inf;
    Position pos_actual = origin;
    while ([](std::vector<bool> v)
           {
        for (bool b : v) {
            if (!b) return true;
        }
        return false; }(consultado))
    {
        minimo_aux = inf;
        for (int j = 0; j < enfermo.size(); j++){
            if(!consultado[j]){
                int valor_actual = manhatan_distance(pos_actual, enfermo[j]);
                if (minimo_aux >= valor_actual){
                    minimo_aux = valor_actual;
                    indice_auxiliar = j;
                }
            }
        }

        pos_actual = enfermo[indice_auxiliar];
        consultado[indice_auxiliar] = true;
        indices.push_back(indice_auxiliar);
    }
    return indices;
}

maximo max_ill(State s, std::vector<Position> enfermo) {
    // Devuelve el enfermo mas lejano
    maximo max{0, {0,0}};
    for(auto pos : enfermo){
        int dis = manhatan_distance(pos, s.ambulance.position);
        if(dis > max.max){
            max.max = dis;
            max.pos = pos;
        }
    }
    return max;
}

int h1(State s, Map map){
    // Si hay enfermos, devuelve la distancia al mas lejano más la distancia al hospital correspondiente
    if (s.contagiosos != 0 || s.no_contagiosos != 0)
    {
        maximo max_cont = max_ill(s, s.contagiosos_pos);
        maximo max_no_cont = max_ill(s, s.no_contagiosos_pos);
        if(max_no_cont.max > max_cont.max){
            Position hosp_nc = map.search_slot(hospital_nc);
            return max_no_cont.max + manhatan_distance(max_no_cont.pos, hosp_nc);
        }
        else{
            Position hosp_c = map.search_slot(hospital_c);
            return max_cont.max + manhatan_distance(max_cont.pos, hosp_c);
            
        }
    }
    // Si no hay enfermos, devuelve la distancia al parking
    else{
        Position park = map.search_slot(parking);
        return manhatan_distance(s.ambulance.position, park);
    }
}

int h2 (State s, Map map) {
    int out = 0; 
    // Si hay enfermos  
    if (s.contagiosos != 0 || s.no_contagiosos != 0) {
        // Si la ambulancia tiene un contagioso devuelve la distancia al contagiosos mas lejano mas la distancia al hospital correspondiente
        if (s.ambulance.cont_contagioso == 1) {
            maximo max_cont = max_ill(s, s.contagiosos_pos);
            out += max_cont.max + manhatan_distance(max_cont.pos, map.cc);
        // Si la ambulancia tiene dos contagiosos devuelve la distancia al hospital de contagiosos
        } else if (s.ambulance.cont_contagioso == 2) {
            out += manhatan_distance(s.ambulance.position, map.cc);
        // Si no tiene contagiosos
        } else {
            // Si tiene 10 no contagiosos devuelve la distancia al hospital de no contagiosos
            if (s.ambulance.cont_no_contagioso == 10) {
                out += manhatan_distance(s.ambulance.position, map.cn);
            // Si tiene 9 no contagiosos devuelve la distancia al no contagioso mas lejano mas la distancia al hospital de no contagiosos
            }else if (s.ambulance.cont_no_contagioso == 9) {
                maximo max_ncont = max_ill(s, s.no_contagiosos_pos);
                out += max_ncont.max + manhatan_distance(max_ncont.pos, map.cn);
            }
            // Si no devuelve la distancia al enfermo más lejano mas la distancia al hospital correspondiente
            else {
            
                maximo max_nc = max_ill(s, s.no_contagiosos_pos);
                maximo max_cont = max_ill(s, s.contagiosos_pos);
                if (max_nc.max >= max_cont.max) {
                    out += max_nc.max + manhatan_distance(max_nc.pos, map.cn);
                } else {
                    out += max_cont.max + manhatan_distance(max_cont.pos, map.cc);
                }
            }
        }
    }
    // Si no hay enfermos devuelve la distancia al parking
    else{
        out += manhatan_distance(s.ambulance.position, map.park);    
    }
    return out;
    
}

int h3(State s, Map map){
    // Si hay enfermos devolverá la distancia al enfermo más lejano más la distancia al hospital correspondiente más la distancia al parking
    if (s.contagiosos != 0 || s.no_contagiosos != 0)
    {
        maximo max_cont = max_ill(s, s.contagiosos_pos);
        maximo max_no_cont = max_ill(s, s.no_contagiosos_pos);
        if(max_no_cont.max > max_cont.max){
            Position hosp_nc = map.search_slot(hospital_nc);
            return max_no_cont.max + manhatan_distance(max_no_cont.pos, hosp_nc) + manhatan_distance(hosp_nc,map.park);
        }
        else{
            Position hosp_c = map.search_slot(hospital_c);
            return max_cont.max + manhatan_distance(max_cont.pos, hosp_c) + manhatan_distance(hosp_c,map.park);
            
        }
    }
    // Si no hay enfermos devolverá la distancia al parking
    else{
        Position park = map.search_slot(parking);
        return manhatan_distance(s.ambulance.position, park);
    }
}

int h4(State s, Map map){
    // Si hay enfermos
    if (s.contagiosos != 0 || s.no_contagiosos != 0)
    {
        // Si la ambulancia tiene 2 contagiosos y 8 no contagiosos devuelve la distancia al hospital más lejano más la distancia al parking
        if(s.ambulance.cont_contagioso == 2 && s.ambulance.cont_no_contagioso == 8){
            int dis_hosp_nc = manhatan_distance(s.ambulance.position, map.cn);
            int dis_hosp_c = manhatan_distance(s.ambulance.position, map.cc);
            if(dis_hosp_c > dis_hosp_nc){
                return dis_hosp_c + manhatan_distance(map.cc, map.park);
            }
            else{
                return dis_hosp_nc + manhatan_distance(map.cn, map.park);
            }
        }
        // Si la ambulancia tiene 10 no contagiosos devuelve la distancia al hospital de no contagiosos más la distancia al parking
        else if(s.ambulance.cont_no_contagioso == 10){
            return manhatan_distance(s.ambulance.position, map.cn) + manhatan_distance(map.cn, map.park);
        }
        // Si no devuelve la distancia del enfermo más lejano más la distancia al hospital correspondiente más la distancia al parking
        else{
            maximo max_cont = max_ill(s, s.contagiosos_pos);
            maximo max_no_cont = max_ill(s, s.no_contagiosos_pos);
            if(max_no_cont.max > max_cont.max){
                Position hosp_nc = map.search_slot(hospital_nc);
                return max_no_cont.max + manhatan_distance(max_no_cont.pos, hosp_nc) + manhatan_distance(hosp_nc,map.park);
            }
            else{
                Position hosp_c = map.search_slot(hospital_c);
                return max_cont.max + manhatan_distance(max_cont.pos, hosp_c) + manhatan_distance(hosp_c,map.park);
                
            }
        }
    }
    // Si no hay enfermos devuelve la distancia al parking
    else{
        Position park = map.search_slot(parking);
        return manhatan_distance(s.ambulance.position, park);
    }
}

int h5(State s, Map m){
    std::vector<Position> enfermos = s.contagiosos_pos;
    Position prev = s.ambulance.position;
    enfermos.insert(enfermos.end(), s.no_contagiosos_pos.begin(), s.no_contagiosos_pos.end());
    std::vector<int> indices = get_vector_min_dist(prev, enfermos);
    int out = 0;
    // En out acumula las distancias de los enfermos
    for (int index : indices)
    {
        Position current = enfermos[index];
        out += manhatan_distance(prev, current);
        prev = current;
    }
    // Si hay enfermos, suma la distancia al hospital más cercano más la distancia al parking
    if (enfermos.size() > 0) {
        Position hosp_c = m.search_slot(hospital_c);
        Position hosp_nc = m.search_slot(hospital_nc);

        int distance_c = manhatan_distance(enfermos[indices[indices.size() - 1]], hosp_c);
        int distance_nc = manhatan_distance(enfermos[indices[indices.size() - 1]], hosp_nc);
        if (distance_c > distance_nc) {
            out += distance_c + manhatan_distance(hosp_c, m.park);
        } else {
            out += distance_nc + manhatan_distance(hosp_nc, m.park);
        }
    // Si no hay enfermos, suma la distancia al parking
    } else (out += manhatan_distance(s.ambulance.position, m.park));
    

    return out;
}
    std::vector<State> sucesors(State current, Map & map)
    {
        std::vector<State> neighbors_aux(4, current);
        std::vector<State> neighbors;

        // Genero los sucesores
        std::vector<bool> aplicable(4, false);
        aplicable[0] = neighbors_aux[0].move_right(map);
        aplicable[1] = neighbors_aux[1].move_left(map);
        aplicable[2] = neighbors_aux[2].move_up(map);
        aplicable[3] = neighbors_aux[3].move_down(map);
        for (int i = 0; i < neighbors_aux.size(); i++)
        {
            if (aplicable[i])
                neighbors.push_back(neighbors_aux[i]);
        }
        return neighbors;
}


std::vector<State> reconstruct_path(std::unordered_map<std::string, State> previo, Element goal) {
    State s = goal.s;
    coste = goal.g;
    std::vector<State> path;
    path.push_back(s);
    while (previo[s.to_string()] != s) {
        path.push_back(previo[s.to_string()]);
        s = previo[s.to_string()];
    }
    return path;
}

std::vector<State> a_star(State origin, State final, std::function<int(State,Map)> h, Map & map) {
    Heap abierta(10'000);
    std::unordered_map<std::string, State> previo; 
    Cerrada cerrada; 
    previo[origin.to_string()] = origin;

    abierta.insert(origin, 0, h(origin, map));
    cerrada.insert(origin, 0, h(origin, map));


    while (!abierta.is_empty()) {
        Element current = abierta.pop(); // extracción del primer elemento de abierta
        nodos++;

        if (current.s.compare_final(final)){
            
            return reconstruct_path(previo, current);
        }
        
        
        std::vector<State> S = sucesors(current.s, map); 

        for (State s : S) {
            if (!cerrada.g.contains(s.to_string())) { // si no se ha explorado antes ese nodo, su g es infinita 
                cerrada.g[s.to_string()] = inf;
            }
            int step_cost = map.get_slot(s.ambulance.position.x, s.ambulance.position.y).get_cost();
            int temp_g = cerrada.g[current.s.to_string()] + step_cost;
            
            if (temp_g < cerrada.g[s.to_string()]) {
                previo[s.to_string()] = current.s;
                cerrada.insert(s, temp_g, temp_g + h(s, map));
                abierta.insert(s, temp_g, temp_g + h(s, map));
            }

        }

    }


    return {};
}



void output_stream(std::vector<State> path, Map & map, char** argv) {
    std::ofstream out_file(map.name + "-" + argv[2] + ".output");
    for (int i = path.size() - 1; i >= 0; --i) {
        out_file << "(" << path[i].ambulance.position.x + 1 << ", " << path[i].ambulance.position.y + 1 << "):" << map.get_slot(path[i].ambulance.position.x, path[i].ambulance.position.y).get_type_str() << ":" <<path[i].ambulance.energy << "\n";
    }
}

void stat_file(float duration, int coste, int longitud, int nodos, std::string name, char** argv) {
    std::ofstream stat_file(name + "-" + argv[2] + ".stat", std::ofstream::out);
    stat_file 
        << "Tiempo total: " << duration << "\n"
        << "Coste total: " << coste << "\n"
        << "Longitud del plan: " << longitud << "\n"
        << "Nodos expandidos: " << nodos;

}


int main (int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Invalid arguments\n";
        return -1;
    }
    auto time = std::chrono::steady_clock::now();
    Map map(argv[1]);
    State origin(map);
    State final(map);
    final.set_final();
    int heuristic = stoi(std::string(argv[2]));
    std::vector<State> path;
    switch (heuristic)
    {
    case 1:
        path = a_star(origin, final, h1, map);
        break;
    case 2:
        path = a_star(origin, final, h2, map);
        break;
    case 3:
        path = a_star(origin, final, h3, map);
        break;
    case 4:
        path = a_star(origin, final, h4, map);
        break;
    case 5:
        path = a_star(origin, final, h5, map);
        break;
    default:
        std::cerr << "Invalid heuristic\n";
        return -2;
        break;
    }
    auto time2 = std::chrono::steady_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time).count() / (float) 1000;
    longitud= path.size();

    output_stream(path, map, argv);
    stat_file(duration, coste, longitud, nodos, map.name, argv);


    return 0;
}