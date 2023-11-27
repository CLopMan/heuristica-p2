import csv

class Read_Write():
    def __init__(self, in_path):
        self.input_path: str = in_path
        self.output_path: str = in_path + ".csv"


    def read(self):
        with open(self.input_path, "r") as file:
            grid: tuple = self.read_grid_size(file)
            pe: list = self.read_electric_places(file)
            var: tuple = self.read_ambulances(file)
        return grid, pe, var[0], var[1]
        
    def read_grid_size(self, file) -> tuple:
        parse = ['x', '\n']
        grid = [0, 0]
        c = ''
        for i in range(2):
            power = 1
            c = file.read(1)
            while c != parse[i]:
                grid[i] = grid[i]*power + int(c)
                power = 10
                
                c = file.read(1)
            if grid[i] == 0:
                raise Exception("Bad input file: grid size")
        return tuple(grid)
    
    def read_electric_places(self, file) -> list:
        line = file.readline() # read a complete line
        line = line[3:-1] # discards PE: and \n
        if len(line) % 5 != 0:
            raise Exception("Bad input file: PE incomplete")
        places = [] 
        for i in range(0, len(line), 5):
            place = line[i: i + 5]
            places.append((int(place[1]), int(place[3])))

        return places
    
    def read_ambulances(self, file) -> tuple:
        out = {
            "TSU-C": [],
            "TSU-X": [],
            "TNU-C": [],
            "TNU-X": [],
        }
        white_spaces = {}

        ambulance = None
        while True:
            ambulance = file.readline()
            if not ambulance:
                return out, white_spaces
            out[ambulance[2:-1]].append(ambulance[:-1])
            white_spaces[ambulance[:-1]] = '-'+ambulance[0]
            
        
    def write(self, data):
        with open(self.output_path, "w") as ofile:
            writer = csv.writer(ofile)
            writer.writerows(data)
