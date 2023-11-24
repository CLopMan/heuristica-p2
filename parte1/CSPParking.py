from read_write import Read_Write

from random import randint
from constraint import *
import sys

def gen_matrix_one_solution(matrix: list, solution:dict, grid):
    #print("one solution:", solution)
    for i in range(grid[0]):
        row = []
        for j in range(grid[1]):
            row.append("-")
        matrix.append(row)
    for ambulance in solution:
        matrix[solution[ambulance][0] - 1][solution[ambulance][1] - 1] = ambulance

def gen_output_data(solutions: list, grid: tuple):
    impresas = []
    super_matrix = []
    
    if len(solutions) > 0:
        for i in range(min(3, len(solutions))):
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
    
    


def free_side(amb1, amb2):
    if amb1[1] != amb2[1]:
        return True
    return amb1[0] != amb2[0] + 1 and amb1[0] != amb2[0] + 1

def tsu_not_on_left(tnu: tuple, tsu:tuple):
    if tnu[0] != tsu[0]:
        return True
    return tsu[1] > tnu[1]

def set_constraints(problem:Problem, ambulances:dict):
    # Every ambulance with other every ambulance
    for i in ambulances:
        for j in ambulances:
            for e in ambulances[i]:
                for ee in ambulances[j]:
                    if e != ee:
                        # All diferent
                        problem.addConstraint(AllDifferentConstraint(), [e, ee])
                        # all ambulance has to have a free place on its side
                        problem.addConstraint(free_side, [e, ee])

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
    
    problem = Problem()
    set_up_problem(problem, grid, ambulances, pe)

    solutions = problem.getSolutions()

    rw.write(gen_output_data(solutions, grid))

if __name__=="__main__":
    main()