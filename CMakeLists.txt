cmake_minimum_required(VERSION 3.20)
project(GoL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(gol 
    src/main.cpp
    src/core/Grid.cpp
    src/simulation/Simulator.cpp
    src/rendering/Window.cpp
    src/rendering/Shader.cpp
    src/rendering/Renderer.cpp
    src/rendering/Camera.cpp
)
target_include_directories(gol PRIVATE include)
target_link_libraries(gol glfw OpenGL::GL GLEW::GLEW)

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

option(COVERAGE "Enable code coverage" OFF)
if(COVERAGE)
    target_compile_options(tests PRIVATE --coverage)
    target_link_options(tests PRIVATE --coverage)

    find_program(LCOV lcov REQUIRED)

    add_custom_target(coverage
        COMMAND ${LCOV} --directory . --zerocounters
        COMMAND $<TARGET_FILE:tests>
        COMMAND ${LCOV} --directory . --capture --output-file coverage.info --ignore-errors mismatch,mismatch
        COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/_deps/*' --output-file coverage.info
        COMMAND ${LCOV} --summary coverage.info
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS tests
    
    )
endif()