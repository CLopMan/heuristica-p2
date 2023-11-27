from read_write import Read_Write

from random import randint
from constraint import *
import sys

NUMBER_OF_SOLUTIONS_MIN = 3

def gen_matrix_one_solution(matrix: list, solution:dict, grid):
    #print("one solution:", solution)
    for i in range(grid[0]):
        row = []
        for j in range(grid[1]):
            row.append("-")
        matrix.append(row)
    for ambulance in solution:
        #print("INDEX: ", solution[ambulance][0] - 1, solution[ambulance][1] - 1)
        if not ambulance[0] == '-':
            matrix[solution[ambulance][0] - 1][solution[ambulance][1] - 1] = ambulance

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
    
    


def free_side(space, amb):
    if space[1] != amb[1]:
        return False 
    return space[0] == amb[0] + 1 or space[0] == amb[0] - 1

"""
casos que ahora mismo se consideran válidos, pero que no debería serlo
caso: restricción a 3                       | caso: restricción con el límite       
- A cumple la restricción con B             |    - B cumple la restricción con A    
- C cumple la restricción con C             |    - A cumple la restricción con B    
- A B y C no cumplen las restricciones      |    - A no tiene hueco                 
                                            |  limite                               
A                                           |  A                                    
B                                           |  B                                    
C                                           |  -                                     
==================================================================================
Casos raros
| caso: plaza libre pero otra no
|                               
|                               
| límite limite limite limite   
| A       - A   -               
| - B     B -   B               
|               C               
|               - D             


Idea: modelar los espacios en blanco

"""

def tsu_not_on_left(tnu: tuple, tsu:tuple):
    if tnu[0] != tsu[0]:
        return True
    return tsu[1] > tnu[1]

def set_constraints(problem:Problem, ambulances:dict, white_spaces: dict):
    # Every ambulance with other every ambulance
    for i in ambulances:
        for e in ambulances[i]:
            # free place right next to the ambulance
            problem.addConstraint(free_side, [white_spaces[e], e])
            for j in ambulances:
                for ee in ambulances[j]:
                    problem.addConstraint(AllDifferentConstraint(), [white_spaces[e], ee])
                    if e != ee:
                        # All diferent
                        problem.addConstraint(AllDifferentConstraint(), [e, ee])
                        # no puede ocupar la misma posición que una ambulancia 
    
        
    # TNU cannot have tsu on left
    tsus = ambulances["TSU-X"] + ambulances["TSU-C"]
    tnus = ambulances["TNU-X"] + ambulances["TNU-C"]
    for tnu in tnus:
        for tsu in tsus:
            problem.addConstraint(tsu_not_on_left, [tnu, tsu])

def gen_domain_grid(grid):
    domain = []
    for i in range(1, grid[0] + 1):
        for j in range(1, grid[1] + 1):
            domain.append((i, j))
    return domain

def set_up_problem(problem: Problem, grid:tuple, ambulances: dict, pe:list, white_spaces: dict):
    complete_grid = gen_domain_grid(grid)

    # Variables - initialization 
    problem.addVariables(ambulances["TSU-C"], pe)
    problem.addVariables(ambulances["TSU-X"], complete_grid)
    problem.addVariables(ambulances["TNU-C"], pe)
    problem.addVariables(ambulances["TNU-X"], complete_grid)
    for ambulance in white_spaces:
        problem.addVariable(white_spaces[ambulance], complete_grid) # el dominio de los espacios en blanco es cualquier espacio 

    # constraints 
    set_constraints(problem, ambulances, white_spaces)


def main():
    rw = Read_Write(sys.argv[1])
    grid, pe, ambulances, white_spaces = rw.read()
    
    problem = Problem()
    set_up_problem(problem, grid, ambulances, pe, white_spaces)

    solutions = problem.getSolutions()

    rw.write(gen_output_data(solutions, grid))

if __name__=="__main__":
    main()