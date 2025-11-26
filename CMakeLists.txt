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

# googletest stuff
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/test_grid.cpp
    tests/test_sim.cpp
    src/core/Grid.cpp
    src/simulation/Simulator.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
