from read_write import Read_Write

from random import randint
from constraint import *
import sys
import itertools

NUMBER_OF_SOLUTIONS_MIN = 3
grid_size: tuple = ()

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
    
# EVALUACION POSICION LIBRE AL LADO EN UN BORDE
def eval_free_side_border(amb1, amb2):
    if amb1[1] == amb2[1]:
        return amb1[0] + 1 != amb2[0] and amb1[0] - 1 != amb2[0]

# POSICION LIBRE AL LADO 
def free_sidea3(amb1, amb2, amb3):
    if amb1[1] == amb2[1] == amb3[1]:
        if amb3[0] == amb2[0] + 1 == amb1[0] + 2:
            return False 
    return True

# POSICION LIBRE AL LADO, EN UN BORDE
def free_side_border(amb1, amb2):
    if (amb1[0] == 0 or amb1[0] == grid_size[0] - 1):
            return eval_free_side_border(amb1, amb2)
    
    return True


def tsu_not_on_left(tnu: tuple, tsu:tuple):
    if tnu[0] != tsu[0]:
        return True
    return tsu[1] > tnu[1]

def set_constraints(problem:Problem, ambulances:dict):
    # Every ambulance with other every ambulance
    for i in ambulances:
        for e in ambulances[i]:
            # free place right next to the ambulance
            for j in ambulances:
                for ee in ambulances[j]:
                    if e != ee:
                        # All diferent
                        problem.addConstraint(AllDifferentConstraint(), [e, ee])
                        problem.addConstraint(free_side_border, [e, ee])
    

    # TNU cannot have tsu on left
    tsus = ambulances["TSU-X"] + ambulances["TSU-C"]
    tnus = ambulances["TNU-X"] + ambulances["TNU-C"]
    for tnu in tnus:
        for tsu in tsus:
            problem.addConstraint(tsu_not_on_left, [tnu, tsu])
    ambulances = tsus + tnus

    for i in itertools.permutations(ambulances, 3):
        problem.addConstraint(free_sidea3, list(i))




def gen_domain_grid(grid):
    domain = []
    for i in range(1, grid[0] + 1):
        for j in range(1, grid[1] + 1):
            domain.append((i, j))
    return domain

def set_up_problem(problem: Problem, grid:tuple, ambulances: dict, pe:list):
    complete_grid = gen_domain_grid(grid)

    # Variables - initialization 
    problem.addVariables(ambulances["TSU-C"], pe)
    problem.addVariables(ambulances["TSU-X"], complete_grid)
    problem.addVariables(ambulances["TNU-C"], pe)
    problem.addVariables(ambulances["TNU-X"], complete_grid)
    # constraints 
    set_constraints(problem, ambulances)


def main():
    rw = Read_Write(sys.argv[1])
    grid, pe, ambulances = rw.read()
    global grid_size
    grid_size = grid
    
    problem = Problem()
    set_up_problem(problem, grid, ambulances, pe)

    solutions = problem.getSolutions()

    rw.write(gen_output_data(solutions, grid))

if __name__=="__main__":
    main()