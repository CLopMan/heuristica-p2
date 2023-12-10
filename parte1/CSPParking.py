from read_write import Read_Write

from random import randint
from constraint import *
import sys
import itertools

NUMBER_OF_SOLUTIONS_MIN = 3 # número de soluciones que se detallan
grid_size: tuple = () # tamaño del mapa

# ======================= print ==============================

# matriz con una solución con el formato adecuado
def gen_matrix_one_solution(matrix: list, solution:dict, grid):
    for i in range(grid[0]):
        row = []
        for j in range(grid[1]):
            row.append("-")
        matrix.append(row)
    for ambulance in solution:
        if not ambulance[0] == '-':
            matrix[solution[ambulance][0] - 1][solution[ambulance][1] - 1] = ambulance

# genera soluciones para escribir en el archivo de salida
def gen_output_data(solutions: list, grid: tuple):
    impresas = []
    super_matrix = []
    
    if len(solutions) > 0:
        for i in range(min(NUMBER_OF_SOLUTIONS_MIN, len(solutions))):
            matrix = []
            random = randint(0, len(solutions) - 1)
            while random in impresas:
                random = randint(0, len(solutions) - 1)
            impresas.append(random)
            gen_matrix_one_solution(matrix, solutions[random], grid)
            matrix.append(list("*"*grid[1]))
            super_matrix += matrix

    first_row = ["Nº Sol:", len(solutions)]
    super_matrix.insert(0, first_row)
    return super_matrix
    

# ====================== CONSTRAINTS ============================

# 3 ambulancias no pueden ser consecutivas en una misma columna
def free_sidea3(amb1, amb2, amb3):
    x = [amb1[0], amb2[0], amb3[0]]
    x.sort()
    if amb1[1] == amb2[1] == amb3[1]:
        if x[2] == x[1] + 1 == x[0] + 2:
            return False 
    return True

# 2 ambulancias no pueden ser consecutivas cuando están en la misma columna y junto a un extremo
def eval_free_side_border(amb1, amb2):
    if amb1[1] == amb2[1]:
        return amb1[0] + 1 != amb2[0] and amb1[0] - 1 != amb2[0]
    return True

def free_side_border(amb1, amb2):
    if (amb1[0] - 1 == 0 or amb1[0] == grid_size[0]):
            return eval_free_side_border(amb1, amb2)
    
    return True

# Una tnu no puede tener una tsu detrás
def tsu_not_on_left(tnu: tuple, tsu:tuple):
    if tnu[0] != tsu[0]:
        return True
    return tsu[1] > tnu[1]

# añade las restricciones
def set_constraints(problem:Problem, ambulances:dict):
    
    # listas de ambulancias
    tsus = ambulances["TSU-X"] + ambulances["TSU-C"] # sólo tsu
    tnus = ambulances["TNU-X"] + ambulances["TNU-C"] # sólo tnu
    ambulances = tsus + tnus # todas

    problem.addConstraint(AllDifferentConstraint()) # todas diferentes

    for i in itertools.permutations(ambulances, 2):
        # plaza libre al lado cuando están junto a un borde
        problem.addConstraint(free_side_border, list(i))
    
    for i in itertools.combinations(ambulances, 3):
        # plaza libre al lado
        problem.addConstraint(free_sidea3, list(i)) 

    # Una tnu no puede tener una tsu detrás
    for tnu in tnus:
        for tsu in tsus:
            problem.addConstraint(tsu_not_on_left, [tnu, tsu])

    


# =============================== Miscelaneous ======================

def gen_domain_grid(grid):
    domain = []
    for i in range(1, grid[0] + 1):
        for j in range(1, grid[1] + 1):
            domain.append((i, j))
    return domain

# ================== problem inicialization =========================

def set_up_problem(problem: Problem, grid:tuple, ambulances: dict, pe:list):
    complete_grid = gen_domain_grid(grid) # dominio de todas las posiciones

    # Variables - inicialización 
    problem.addVariables(ambulances["TSU-C"], pe)
    problem.addVariables(ambulances["TSU-X"], complete_grid)
    problem.addVariables(ambulances["TNU-C"], pe)
    problem.addVariables(ambulances["TNU-X"], complete_grid)
    
    # constraints 
    set_constraints(problem, ambulances)


# ============================ Main ===================================
def main():
    # objeto encargado de la lectura y escritura
    rw = Read_Write(sys.argv[1])

    # datos de lectura
    grid, pe, ambulances = rw.read()
    global grid_size
    grid_size = grid
    
    problem = Problem()

    # inicialización del problema
    set_up_problem(problem, grid, ambulances, pe)

    solutions = problem.getSolutions()
    rw.write(gen_output_data(solutions, grid))

if __name__=="__main__":
    main()