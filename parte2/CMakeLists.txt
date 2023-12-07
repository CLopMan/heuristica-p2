# General
cmake_minimum_required(VERSION 3.25)
project(parte2 LANGUAGES CXX)
add_executable(parte2 
                ambulance.cpp
                map.hpp
                map.cpp
                main.cpp 
                state.hpp 
                heap.cpp
                )

# Estandar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opciones de compilacion
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)


# incluir todos los directorios de código
include_directories(PUBLIC .)