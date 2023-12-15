import os
import sys

def main():
    # compilation
    os.system("cmake -S src -B build -DCMAKE_BUILD_TYPE=Release > /dev/null")
    os.system("cmake --build build > /dev/null")

    # Execution
    os.system("./build/parte2 " + sys.argv[1] + " " + sys.argv[2])

if __name__ == "__main__":
    main()