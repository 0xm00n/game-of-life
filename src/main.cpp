#include "simulation/Simulator.h"
#include <iostream>

void print_grid(const Grid& grid) {
    for (int y = 0; y < grid.get_height(); y++) {
        for (int x = 0; x < grid.get_width(); x++) {
            std::cout << (grid.is_alive(x, y) ? "█" : "·");
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    Simulator sim = Simulator(100, 100);

    sim.set_cell(5, 4, true); 
    sim.set_cell(5, 5, true); 
    sim.set_cell(5, 6, true);  

    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());
    sim.step();
    print_grid(sim.get_grid());

}
