cmake_minimum_required(VERSION 3.20)
project(GoL LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(gol 
    src/main.cpp
    src/core/Grid.cpp
    src/simulation/Simulator.cpp
)
target_include_directories(gol PRIVATE include)
